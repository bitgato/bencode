# bencode
---
A simple bencode parser which uses hash tables for dictionaries.
Hash algorithm used is FNV-1a (32 bit).
Also note that all string related data is read/returned as `unsigned char*`
and not as `char*`.

**NOTE: You need the openssl library installed so that info_hash can be
calculated**

### decoding a bencoded torrent file
```C
be_dict *dict = decode_file(const char *file);
```
This will return the outermost dictionary in the torrent file or `NULL` if an
error occurs. Note that this does not differentiate between the cases where
the file doesn't exist or a syntax error occurs.

### decoding from buffer
```C
unsigned char *buffer;
size_t len;
be_type type;
// ... Initialize the buffer and length
be_dict *dict = decode(&buffer, &len, &type);
```
This will return `NULL` only when a syntax error is present. This way, you can
check for syntax errors and problems with reading files separately. But you
will need to check the `type` manually.

### freeing the created dictionary
Should always be done after the dictionary is used. There's no need to
`free` individual values or lists. Do this once after everything is done.
```C
dict_destroy(dict);
```

### getting the value of a key
```C
be_type type;
void *val = dict_get(be_dict *dict, unsigned char *key, be_type *type);
```
Returns the value of the key or `NULL` if it doesn't exist. Stores the type
of the value in the `be_type` pointer provided. You can cast the returned
value depending on `type`.

### lists
Linked lists are used for storing lists here. Iterating over a list which
you got from a dictionary can be done by:
```C
be_type type;
be_list *list = (be_list*)dict_get(dict, key, &type);
while(list != NULL) {
	be_node node = list->node;
	void *node_val = node.val;
	be_type type = node.type;
	list = list->next;
}
```
You generally don't need to manually free a list. It is handled by the
`dict_destroy` function.

### strings
Strings are stored in a `struct` that contain the string itself and the
length of the string.
```C
be_type type;
be_string *string = (be_string*)dict_get(dict, key, &type);
size_t len = string->len;
unsigned char *val = string->str;
```

### integers
Integers are stored as `long long int` here.
```C
be_type type;
long long int i = (long long int)dict_get(dict, key, &type);
```

### info hash
The info hash of the torrent file is stored in the `struct be_dict` as an
array of 3-byte-long strings. Each element is a hex representation of the
SHA1 hashed byte (2 byte long) (`241`(dec) will be stored as `F1`(hex))
followed by a null terminator. So actually `241` will be stored as `F1\0`.
Using info_hash:
```C
// SHA_DIGEST_LENGTH can be replaced by 20
for(int i=0; i<SHA_DIGEST_LENGTH; ++i) {
	// hex representation
	char *hex_rep = dict->info_hash[i];
	if(i>0) printf(":");
	printf("%s", hex_rep);
}
```
Output will be of the form `F1:FC:DC:14:62:D3:65:30:F5:26:C1:D9:40:2E:EC:91:00:B7:BA:18`;

### running the tests
The `tests/test.c` is more of a demonstration file. You should have a look
to see how some things work. You can compile and run it by:
```shell
cd tests
gcc -Wall -g test.c ../bencode.c ../bencode.h ../dict.c -o test -lssl -lcrypto
valgrind ./test
```
Using valgrind to detect any memory leaks is recommended. Please report if
you find any.

### note
Be sure to check all returned values for `NULL` and confirm the types before
actually using the values further. You could also store the returned `void*`,
check the type and *then* cast it.

---
Please read `bencode.h` for documentation on other functions
(which you may or may not need).
