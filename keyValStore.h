#ifndef BSGRUPPE25PRAKTIKUM_KEYVALSTORE_H
#define BSGRUPPE25PRAKTIKUM_KEYVALSTORE_H

/// Adds a key value pair to the storage. Creates a storage if called for the first time.
/// \param key Key of the new pair
/// \param value value of the new pair
/// \return -1 => No space in the storage\n 0 => Added successfully\n 1 => Key was already in the storage and override the value
int put(char* key, char* value);

/// Retrieve of a pair from the storage. Result gets stored in the char* of res.
/// \param key Key of the pair
/// \param res Pointer to store the result in !!! CAUTION FOR BUFFER OVERFLOW !!!
/// \return -1 => No pair with this key in storage\n 0 => Retrieve successfully
int get(char* key, char* res);

/// Deletion of a pair.
/// \param key Key of the pair to delete
/// \return -1 => No pair with this key in storage\n 0 => Deletion successfully
int del(char* key);

#endif //BSGRUPPE25PRAKTIKUM_KEYVALSTORE_H
