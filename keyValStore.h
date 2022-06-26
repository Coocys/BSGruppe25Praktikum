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
/// \return -1 => No pair with this key in storage\n 0 => Retrieve successfull
int get(char* key, char* res);

/// Deletion of a pair.
/// \param key Key of the pair to delete
/// \return -1 => No pair with this key in storage\n 0 => Deletion successfull
int del(char* key);

/// Adds subscriber to keys subscriber list
/// \param key Key of the pair to subscribe to
/// \param subscriber Identifying number of client_socket that subscribes
/// \return -1 => No pair with this key in storage\n 0 => Subscription successfull
int sub(char* key, int subscriber);

/// Get list of subscribers. Result is stored in the int* subscribers.
/// \param key Key of the pair to changed that subscribers need to be notified about
/// \param subscribers Pointer to array in main
/// \return -1 => No pair with this key in storage\n 0 => Publishing successfull
int pub(char* key, int* subscribers);

/// Removes disconnected client from subscription lists.
/// \param subscriber Identifying number of client_socket that disconnected
/// \return -1 => No pair with this key in storage\n 0 => Subscription successfull
void removeSub(int subscriber);



#endif //BSGRUPPE25PRAKTIKUM_KEYVALSTORE_H
