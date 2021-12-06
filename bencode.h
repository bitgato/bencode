#ifndef BENCODE_H
#define BENCODE_H

#include <stddef.h>

#define INIT_CAP 32
#define FNV_OFF 2166136261UL // FNV1a 32 bit offset
#define FNV_PRIME 16777619UL // FNVI1a 32 bti prime

typedef enum
{
	BE_INT,
	BE_STR,
	BE_LIST,
	BE_DICT,
} be_type;

typedef struct be_node
{
	be_type type;
	unsigned char *key;
	void *val;
} be_node;

typedef struct be_string
{
	size_t len;
	unsigned char *str;
} be_string;

typedef struct be_list
{
	be_node node;
	struct be_list *next;
	// So that we can add elements easily and preserve order
	struct be_list *last;
} be_list;

typedef struct be_dict
{
	size_t capacity;
	size_t length;
	struct be_node *entries;
} be_dict;

/**
 * Decodes a bencoded file
 * @param file The path of the bencoded file
 * @return The outer dictionary or NULL if the file is not
 * properly bencoded. The returned dicitonary should be free'd
 * with dict_destroy after use
 * @see dict_destroy
 */
be_dict *decode_file (const char *file);

/**
 * Creates an empty dictionary. The dictionary needs to be freed by
 * calling dict_destroy
 * @return Created dictionary or NULL if an error occurs
 */
be_dict *dict_create (void);

/**
 * Frees all dynamically allocated space in a given dictionary
 * @param dict The dictionary to be freed
 */
void dict_destroy (be_dict *dict);

/**
 * Returns the value of the node with key and stores the variable type
 * into the passed type pointer
 * @param dict The dictionary in which the key is to be searched
 * @param key The key, of which, value is needed
 * @param type The type of the returned value will be stored in this pointer
 * @return The value of the given key or NULL if the key doesn't exist in
 * the dictionary
 */
void *dict_get (be_dict *dict, unsigned char *key, be_type *type);

/**
 * Inserts a key and a value into a given dictionary
 * @param dict The dictionary in which key is to be inserted
 * @param key The key to be inserted
 * @param val The value to be inserted
 * @param type The type of the value provided
 * @return The key provided or NULL if an error occurs
 */
unsigned char *dict_set (be_dict *dict, unsigned char *key, void *val,
	be_type type);

/**
 * Prints a value from a bencode dictionary to STDIN
 * @param key The key of the value to be printed
 * @param val The value to be printed
 * @param type The type of the value to be printed
 */
void dict_val_print (unsigned char *key, void *val, be_type type);

/**
 * Creates a list of bencoded nodes
 * @param node Initial node
 */
be_list *list_create (be_node node);

/**
 * Adds a bencoded node to a list
 * @param list The list to which the node is to be added
 * @param node The node to be added
 */
be_list *list_add (be_list *list, be_node node);

/**
 * Frees a list of bencoded nodes
 * @param list The list to be free'd
 */
void list_free (be_list *list);

/**
 * Prints the values in a dictionary to STDOUT
 * @param dict The dictionary to be dumped
 */
void dict_dump (be_dict *dict);

#endif /* BENCODE_H */
