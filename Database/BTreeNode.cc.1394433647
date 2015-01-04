#include <stdio.h>
#include <fstream>
#include "BTreeNode.h"

using namespace std;

void BTLeafNode::display(PageId pid){
	if(pid != -1){
		ofstream outFile;
	  	outFile.open("nodes_leaf", ios_base::app);
	  	outFile << "Page " << pid << "\n";
	  	outFile << "keycount = " << getKeyCount() << "\n";
	  	outFile << "nextpid = " << getNextNodePtr() << "\n";
	  	outFile << "keys: ";


		int key;
		RecordId rid;
		for(int i = 0; i < getKeyCount(); i++){
			readEntry(i, (int&)key, (RecordId&)rid);
			outFile << key << ", ";
		}
		outFile << "\n\n";
	}
	else{
		printf("Page %d\n", pid);
		printf("keycount = %d\n", getKeyCount());
		printf("nextpid = %d\n", getNextNodePtr());
		printf("keys: ");
		int key;
		RecordId rid;
		for(int i = 0; i < getKeyCount(); i++){
			readEntry(i, (int&)key, (RecordId&)rid);
			printf("%d, ", key);
		}
		printf("\n");
	}
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
	int rc = pf.read(pid, buffer);
	//printf("%d read code\n", rc);
	//printf("epid = %d\n", pf.endPid());
	if(rc == 0){
		//printf("leaf page read\n");
		return 0;
	}
	else{
		//printf("leaf page not read\n");
		return 1;
	}
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{
	int rc = pf.write(pid, buffer);
	//printf("%d leaf pid to write\n", pid);
	//printf("%d leaf write code\n", rc);
	if(rc == 0){
		//printf("leaf page written\n");
		return 0;
	}
	else{
		//printf("leaf page not written\n");
		return 1;
	}
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
	int keycount;
	memcpy(&keycount, buffer, sizeof(int));
	return keycount;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return 1 if the node is full. Return -1 if not found.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{
	int num_keys = getKeyCount();
	if(num_keys == MaxLeafEntries){
		return 1;
	}
	if(num_keys == 0){
		memcpy(buffer + headerSize, &key, sizeof(int));
		memcpy(buffer + headerSize + sizeof(int), &rid, sizeof(RecordId));
		num_keys += 1;
		memcpy(buffer, &num_keys, sizeof(int));
		int test;
		memcpy(&test, buffer + headerSize, sizeof(int));
		return 0;
	}
	int eid;
	int result;
	result = locate(key, (int&) eid);
	if(result != -1){
		BTLeafNode temp;
		int size = headerSize + eid * leafEntrySize;
		memcpy(temp.buffer + size, &key, sizeof(int));
		//temp.addObject(&key, size);
		memcpy(temp.buffer + size + sizeof(int), &rid, sizeof(RecordId));
		//temp.addObject((const void*)(&rid), size + sizeof(int));
		memcpy(temp.buffer + size + sizeof(int) + sizeof(RecordId), buffer + size, (num_keys - eid) * (sizeof(int) + sizeof(RecordId)));
		memcpy(buffer + size, temp.buffer + size, sizeof(int) + sizeof(PageId) + (num_keys + 1) * (sizeof(int) + sizeof(RecordId)));
		//memcpy(&new_keys, buffer, sizeof(int));
		num_keys += 1;
		memcpy(buffer, &num_keys, sizeof(int));
		return 0;
	}
	else{
		return -1;
	}
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{
	//get halfway value
	int num_keys = getKeyCount();
	int key_array [num_keys + 1];
	int displacement = headerSize;
	bool fill = false;
	int i = 0;
	int key_position;
	while((!fill && i < num_keys) || (fill && i <= num_keys)){
		memcpy(key_array + i, buffer + displacement, sizeof(int));
		if(!fill && key_array[i] > key){
			key_array[i + 1] = key_array[i];
			key_array[i] = key;
			key_position = i;
			i++;
			fill = true;
		}
		i++;
		displacement += leafEntrySize;
	}
	if(!fill){
		key_array[num_keys] = key;
		key_position = num_keys;
	}
	int split_position = (int)(ceil((num_keys + 1) / 2));

	//set keycount for new node
	int new_keys = num_keys + 1 - split_position;
	memcpy(sibling.buffer, &new_keys, sizeof(int));

	//copy half the values to new 
	if(key_position >= split_position){
		memcpy(buffer, &split_position, sizeof(int)); //update keycount on old node 
		int shift_keys = key_position - split_position;
		memcpy(sibling.buffer + headerSize, buffer + headerSize + split_position * leafEntrySize, shift_keys * leafEntrySize);
		memcpy(sibling.buffer + headerSize + shift_keys * leafEntrySize, &key, sizeof(int));
		memcpy(sibling.buffer + headerSize + shift_keys * leafEntrySize + sizeof(int), &rid, sizeof(RecordId));
		memcpy(sibling.buffer + headerSize + shift_keys * leafEntrySize + leafEntrySize,
		 	   buffer + headerSize + key_position * leafEntrySize, (num_keys - key_position) * leafEntrySize);
	}
	else{
		BTLeafNode temp;
		memcpy(buffer, &split_position, sizeof(int)); //update keycount on old node 
		memcpy(temp.buffer + headerSize, buffer + headerSize, key_position * leafEntrySize);
		memcpy(temp.buffer + headerSize + key_position * leafEntrySize, &key, sizeof(int));
		memcpy(temp.buffer + headerSize + key_position * leafEntrySize + sizeof(int), &rid, sizeof(RecordId));
		memcpy(temp.buffer + headerSize + key_position * leafEntrySize + leafEntrySize,
		 	   buffer + headerSize + key_position * leafEntrySize, (split_position - key_position - 1) * leafEntrySize);
		memcpy(sibling.buffer + headerSize, buffer + headerSize + (split_position - 1) * leafEntrySize,
			  (num_keys - split_position + 1) * leafEntrySize);
		memcpy(buffer + headerSize, temp.buffer + headerSize, split_position * leafEntrySize);
	}
	siblingKey = key_array[split_position]; //this is sibling_key

	return 0;
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful and key value is greater to searchKey. Return 1 if successful and key value is the same as searchKey
 * 																  Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{
	//refactor using binary search
	int key_check;
	int counter = 0;
	int num_keys = getKeyCount();
	if(num_keys == 0){
		eid = 0;
		return 0;
	}
	int displacement = headerSize;
	memcpy(&key_check, buffer + displacement, sizeof(int));
	while((counter < num_keys) && (key_check < searchKey)){
		counter += 1;
		displacement += leafEntrySize;
		memcpy(&key_check, buffer + displacement, sizeof(int));
	}
	if(counter <= num_keys && (searchKey < key_check)){
		eid = counter;
		return 0;
	}
	else if(counter <= num_keys && (searchKey == key_check)){
		eid = counter;
		return 1;
	}
	else if(num_keys < MaxLeafEntries){
		eid = num_keys;
		return 2;
	}
	else{
		eid = -1;
		return -1;
	}
	
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
	int displacement = headerSize + eid * leafEntrySize;
	memcpy(&key, buffer + displacement, sizeof(int));
	memcpy(&rid, buffer + displacement + sizeof(int), sizeof(RecordId));
	return 0;
}

/*
 * Return the pid of the next sibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
	int next_pid;
	memcpy(&next_pid, buffer + 3 * sizeof(int) + sizeof(PageId), sizeof(int));
	return next_pid;
}

/*
 * Set the pid of the next sibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{
	memcpy(buffer + 3 * sizeof(int) + sizeof(PageId), &pid, sizeof(PageId));
	//addObject((void*) pid, sizeof(int));
	return 0;
}

/*
 * Initialize the leaf node with (key, rid).
 * @param key[IN] the key that should be paired with the RecordID
 * @param ird[IN] the RecordId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */;
RC BTLeafNode::initializeLeaf(int key, const RecordId& rid)
{
	if(rid.pid != -1){
		int keycount = 1;
		PageId next = -1;
		PageId rootpid;
		rootpid = 0;
		int treeheight = 0;
		int total_keys = 1;
		memcpy(buffer, &keycount, sizeof(int));
		memcpy(buffer + sizeof(int), &rootpid, sizeof(PageId));
		memcpy(buffer + sizeof(int) + sizeof(PageId), &treeheight, sizeof(int));
		memcpy(buffer + 2 * sizeof(int) + sizeof(PageId), &total_keys, sizeof(int));
		memcpy(buffer + 3 * sizeof(int) + sizeof(PageId), &next, sizeof(PageId));
		memcpy(buffer + headerSize, &key, sizeof(int));
		memcpy(buffer + headerSize + sizeof(int), (const void*)(&rid), (sizeof(RecordId)));
		//addObject((void*) keycount, 0);
		//addObject((void*) next, sizeof(int));
		//addObject((void*) key, sizeof(keycount) + sizeof(PageId));
		//memcpy((void*) (buffer + headerSize + sizeof(int)), (const void*)(&rid), (sizeof(RecordId)));
	}
	else{
		int keycount = 0;
		PageId next = -1;
		addObject((void*) keycount, 0);
		addObject((void*) next, sizeof(int));
	}
	return 0;
}

RC BTLeafNode::addObject(const void* object, int pos)
{
	memcpy(buffer + pos, &object, sizeof(object));
	return 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{
	if(pf.read(pid, buffer) == 0){
		//printf("non leaf page read\n");
		return 0;
	}
	else{
		//printf("non leaf page read\n");
		return 1;
	}
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{
	int rc = pf.write(pid, buffer);
	//printf("%d nonleaf pid to write\n", pid);
	if(rc == 0){
		//printf("non leaf page written\n");
		return 0;
	}
	else{
		//printf("non leaf page not written\n");
		return 1;
	}
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{
	int keycount;
	memcpy(&keycount, buffer, sizeof(int));
	return keycount;
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{
	//if(){
		int check;
		int i = 0;
		int num_keys = getKeyCount();
		bool found = false;
		while(!found && i < num_keys){
			memcpy(&check, buffer + nonleafheader + i * nonleafEntrySize + sizeof(PageId), sizeof(int));
			if(check >= key){
				BTNonLeafNode temp;
				memcpy(temp.buffer + nonleafheader, buffer + nonleafheader, i * nonleafEntrySize + sizeof(PageId));
				memcpy(temp.buffer + nonleafheader + i * nonleafEntrySize + sizeof(PageId), &key, sizeof(int));
				memcpy(temp.buffer + nonleafheader + i * nonleafEntrySize + nonleafEntrySize, &pid, sizeof(PageId));
				memcpy(temp.buffer + nonleafheader + i * nonleafEntrySize + nonleafEntrySize + sizeof(PageId),
					   buffer + nonleafheader + i * nonleafEntrySize + sizeof(PageId), (num_keys - i) * nonleafEntrySize);
				memcpy(buffer + nonleafheader, temp.buffer + nonleafheader, (num_keys + 1) * nonleafEntrySize + sizeof(PageId));
				found = true;
			}
			i++;
		}
		if(!found){//put it at the end
			memcpy(buffer + nonleafheader + num_keys * nonleafEntrySize + sizeof(PageId), &key, sizeof(int));
			memcpy(buffer + nonleafheader + num_keys * nonleafEntrySize + sizeof(PageId) + sizeof(int), &pid, sizeof(PageId));
		}
		num_keys += 1;
		memcpy(buffer, &num_keys, sizeof(int));
		return 0;
	/*}
	else{
		return 1;
	}*/
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey) //needs to update rootpid and treeheight on root split
{
	//get halfway value
	int num_keys = getKeyCount();
	int key_array [num_keys + 1];
	int displacement = nonleafheader + sizeof(PageId);
	bool fill = false;
	int key_position;
	int i = 0;
	while((!fill && i < num_keys) || (fill && i <= num_keys)){
		memcpy(key_array + i, buffer + displacement, sizeof(int));
		if(!fill && key_array[i] > key){
			key_array[i + 1] = key_array[i];
			key_array[i] = key;
			key_position = i;
			i++;
			fill = true;
		}
		i++;
		displacement += nonleafEntrySize;
	}
	if(!fill){
		key_array[num_keys] = key;
		key_position = num_keys;
	}
	int split_position = (int)(ceil((num_keys + 1.0) / 2)); //may not be doing anything as resolves to int before ceil
	midKey = key_array[split_position];

	//set keycount for new node
	int new_keys = num_keys - split_position;
	memcpy(sibling.buffer, &new_keys, sizeof(int));

	//copy half the values to new 
	if(key_position > split_position){
		memcpy(buffer, &split_position, sizeof(int)); //update keycount on old node 
		int shift_keys = key_position - split_position - 1;
		//are boundary cases correct
		memcpy(sibling.buffer + nonleafheader, buffer + nonleafheader + (split_position + 1) * nonleafEntrySize,
			   shift_keys * nonleafEntrySize + sizeof(PageId));
		memcpy(sibling.buffer + nonleafheader + shift_keys * nonleafEntrySize + sizeof(PageId), &key, sizeof(int));
		memcpy(sibling.buffer + nonleafheader + shift_keys * nonleafEntrySize + nonleafEntrySize, &pid, sizeof(PageId));
		memcpy(sibling.buffer + nonleafheader + shift_keys * nonleafEntrySize + nonleafEntrySize + sizeof(PageId),
		 	   buffer + nonleafheader + (split_position + 1) * nonleafEntrySize + sizeof(PageId), (num_keys - key_position) * nonleafEntrySize);
	}
	else if(key_position == split_position){
		memcpy(buffer, &split_position, sizeof(int)); //update keycount on old node 
		int shift_keys = num_keys - split_position;
		//are boundary cases correct
		memcpy(sibling.buffer + nonleafheader, &pid, sizeof(PageId));
		memcpy(sibling.buffer + nonleafheader + sizeof(PageId), buffer + nonleafheader + split_position * nonleafEntrySize + sizeof(PageId),
			   shift_keys * nonleafEntrySize);
	}
	else{
		BTNonLeafNode temp;
		memcpy(buffer, &split_position, sizeof(int)); //update keycount on old node 
		memcpy(temp.buffer + nonleafheader, buffer + nonleafheader, key_position * nonleafEntrySize + sizeof(PageId));
		memcpy(temp.buffer + nonleafheader + key_position * nonleafEntrySize + sizeof(PageId), &key, sizeof(int));
		memcpy(temp.buffer + nonleafheader + key_position * nonleafEntrySize + nonleafEntrySize, &pid, sizeof(PageId));
		memcpy(temp.buffer + nonleafheader + key_position * nonleafEntrySize + nonleafEntrySize + sizeof(PageId),
		 	   buffer + nonleafheader + key_position * nonleafEntrySize + sizeof(PageId),
		 	  (split_position - key_position - 1) * nonleafEntrySize);
		memcpy(sibling.buffer + nonleafheader, buffer + nonleafheader + split_position * nonleafEntrySize,
			  (num_keys - split_position) * nonleafEntrySize + sizeof(PageId));
		memcpy(buffer + nonleafheader, temp.buffer + nonleafheader, split_position * nonleafEntrySize + sizeof(PageId));
	}
	return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
	int check;
	int num_keys = getKeyCount();
		for(int i = 0; i < num_keys; i++){
			memcpy(&check, buffer + nonleafheader + sizeof(PageId) + i * nonleafEntrySize, sizeof(int));
			if(check == searchKey){
				memcpy(&pid, buffer + nonleafheader + (i + 1) * nonleafEntrySize, sizeof(PageId));
				return 0;
			}
			if(check > searchKey){
				memcpy(&pid, buffer + nonleafheader + i * nonleafEntrySize, sizeof(PageId));
				return 0;
			}
		}
	//last pointer
	memcpy(&pid, buffer + nonleafheader + num_keys * nonleafEntrySize, sizeof(PageId));	
	return 0;	
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
	int keycount = 1;
	PageId root_location;
	root_location = 0;
	int tree_height = 1;
	int total_keys = 1;
	//printf("values to insert: %d, %d, %d, %d\n", keycount, pid1, key, pid2);

	memcpy(buffer, &keycount, sizeof(int));
	memcpy(buffer + sizeof(int), &root_location, sizeof(PageId));
	memcpy(buffer + sizeof(int) + sizeof(PageId), &tree_height, sizeof(int));
	memcpy(buffer + sizeof(int) + sizeof(PageId) + sizeof(int), &total_keys, sizeof(int));	
	memcpy(buffer + nonleafheader, &pid1, sizeof(PageId));
	memcpy(buffer + nonleafheader + sizeof(PageId), &key, sizeof(int));
	memcpy(buffer + nonleafheader + nonleafEntrySize, &pid2, sizeof(PageId));
	return 0;
}

/*
 *Add object to the node's buffer at offset pos
*/
RC BTNonLeafNode::addObject(const void* object, int pos)
{
	memcpy(buffer + pos, &object, sizeof(object));
	return 0;
}

int BTNonLeafNode::getTreeHeight()
{
	int tree_height;
	memcpy(&tree_height, buffer + sizeof(int) + sizeof(PageId), sizeof(int));
	return tree_height;
}

RC BTNonLeafNode::setTreeHeight(int new_height){
	memcpy(buffer + sizeof(int) + sizeof(PageId), &new_height, sizeof(int));
	return 0;
}

PageId BTNonLeafNode::getRootPid()
{
	PageId rootpid;
	memcpy(&rootpid, buffer + sizeof(int), sizeof(PageId));
	return rootpid;
}

int BTNonLeafNode::getTotalKeys()
{
	int total_keys;
	memcpy(&total_keys, buffer + 2 * sizeof(int) + sizeof(PageId), sizeof(int));
	return total_keys;
}

RC BTNonLeafNode::setRootPid(PageId pid){
	memcpy(buffer + sizeof(int), &pid, sizeof(PageId));
	return 0;
}

RC BTNonLeafNode::setTotalKeys(int total_keys){
	memcpy(buffer + 2* sizeof(int) + sizeof(PageId), &total_keys, sizeof(int));
	return 0;
}

void BTNonLeafNode::display(PageId pid){
	if(pid != -1){
		ofstream outFile;
	  	outFile.open("nodes_nonleaf", ios_base::app);
	  	outFile << "Page " << pid << "\n";
	  	outFile << "keycount = " << getKeyCount() << "\n";
	  	outFile << "rootpid = " << getRootPid() << "\n";
	  	outFile << "treeheight = " << getTreeHeight() << "\n";
	  	outFile << "PageId-key pairs: ";

		int key;
		PageId pid;
		for(int i = 0; i < getKeyCount(); i++){
			memcpy(&pid, buffer + nonleafheader + i * nonleafEntrySize, sizeof(PageId));
			memcpy(&key, buffer + nonleafheader + i * nonleafEntrySize + sizeof(PageId), sizeof(int));
			outFile << "(" << pid << ",";
			outFile << key << "), ";
		}
		memcpy(&pid, buffer + nonleafheader + getKeyCount() * nonleafEntrySize, sizeof(PageId));
		outFile << pid << "\n\n";
	}
	else{
		printf("Page %d\n", pid);
		printf("keycount = %d\n", getKeyCount());
		printf("rootpid = %d\n", getRootPid());
		printf("treeheight = %d\n", getTreeHeight());
		printf("PageId-key pairs: ");
		int key;
		RecordId rid;
		for(int i = 0; i < getKeyCount(); i++){
			memcpy(&pid, buffer + nonleafheader + i * nonleafEntrySize, sizeof(PageId));
			memcpy(&key, buffer + nonleafheader + i * nonleafEntrySize + sizeof(PageId), sizeof(int));
			printf("(%d,%d), ", pid, key);
		}
		memcpy(&pid, buffer + nonleafheader + getKeyCount() * nonleafEntrySize, sizeof(PageId));
		printf("%d\n", pid);
	}
}
