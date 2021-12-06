# bencode
---
A simple bencode parser which uses hash tables for dictionaries.
Hash algorithm used is FNV-1a (32 bit).
Also note that all string related data is read/returned as `unsigned char*`
and not as `char*`.

### decoding a bencoded torrent file
```C
be_dict *dict = decode_file(const char *file);
```
This will return the outermost dictionary in the torrent file or `NULL` if an
error occurs. Note that this does not differentiate between the cases where
the file doesn't exist or a syntax error occurs.

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

### note
Be sure to check all returned values for `NULL` and confirm the types before
actually using the values further. You could also store the returned `void*`,
check the type and *then* cast it.

---
Please read `bencode.h` for documentation on other functions
(which you may or may not need).
