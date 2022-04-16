#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/sha.h>

#include "../bencode.h"

bool
test_file(const char *file)
{
	be_dict *dict = decode_file(file);
	if(dict==NULL) {
		printf("Syntax error\n");
		return false;
	}
	else {
		// The hashes for the torrent files provided should be:
		// F1:FC:DC:14:62:D3:65:30:F5:26:C1:D9:40:2E:EC:91:00:B7:BA:18
		// FB:4E:21:C8:42:74:A8:BA:AF:0F:E2:12:18:B5:94:2C:5C:02:14:06
		// 21:DD:64:01:FE:71:25:0D:29:68:03:8D:D6:CA:07:EB:8D:60:DF:49
		// Also check if the dictionary even has a info-hash
		if(dict->has_info_hash) hex_dump(dict->info_hash, SHA_DIGEST_LENGTH);

		void *val; be_type type;
		unsigned char *key = (unsigned char*)"info";
		val = dict_get(dict, key, &type);
		// Can also do the following
		// val = dict_get(dict, (unsigned char*)"info", &type);
		if(val==NULL || type!=BE_DICT) {
			dict_destroy(dict);
			return false;
		}
		// Casting the void pointer after type has been checked
		be_dict *info = (be_dict*)val;

		key = (unsigned char*)"name";
		val = dict_get(info, key, &type);
		if(val==NULL || type!=BE_STR) {
			dict_destroy(dict);
			return false;
		}
		// Note that strings are stored in structs
		be_string *string = (be_string*)val;
		printf("%s\n", string->str);

		// Uncomment the following line to dump the whole dictionary too
		// dict_dump(dict);
	}
	dict_destroy(dict);
	return true;
}

int
main()
{
	int len = 3;
	const char *files[len];
	files[0] = "tests/torrents/ubuntu.torrent";
	files[1] = "tests/torrents/opensuse.torrent";
	files[2] = "tests/torrents/fedora.torrent";
	for(int i=0; i<len; ++i) {
		if(!test_file(files[i])) {
			printf("Failed\n");
			return 1;
		}
		printf("Passed\n");
	}
	return 0;
}
