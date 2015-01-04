/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include "PageFile.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
  RC   rc;
  //char page[PageFile::PAGE_SIZE];

  // open the page file
  if ((rc = pf.open(indexname, mode)) < 0) return rc;

  return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	if(pf.endPid() == 0){
		/*BTNonLeafNode root;
		root.initializeRoot(1, key, 2);
		root.write(0, (PageFile&) pf);
		root.display(0);*/
		BTLeafNode firstleaf;
		firstleaf.initializeLeaf(key, rid);
		firstleaf.write(0, (PageFile&) pf);
		//firstleaf.display(0);
		/*RecordId empty;
		empty.pid = -1;
		BTLeafNode secondleaf;
		secondleaf.initializeLeaf(key, (const RecordId&) empty);
		secondleaf.write(2, (PageFile&) pf);
		secondleaf.display(2);*/
		//treeHeight++;

	}
	else{
		//find which leaf node you're inserting into
		BTNonLeafNode root;
		//BTLeafNode root_leaf;
		root.read(0, (const PageFile&)pf);
		int total_keys = root.getTotalKeys();
		int rootpid = root.getRootPid();
		int treeheight = root.getTreeHeight();
		IndexCursor cursor;
		if(treeheight == 0){
			cursor.pid = 0;
		}
		else{
			root.read(rootpid, (const PageFile&)pf);
			locate(key, (IndexCursor&)cursor);
		}
		BTLeafNode leafnode;
		leafnode.read(cursor.pid, (const PageFile&)pf);
		if(leafnode.getKeyCount() >= MaxLeafEntries){
			BTLeafNode sibling;
			int midKey;
			PageId old_pid = leafnode.getNextNodePtr();
			leafnode.insertAndSplit(key, (const RecordId&)rid, (BTLeafNode&)sibling, (int&)midKey);

			sibling.setNextNodePtr(old_pid);
			PageId end_pid = pf.endPid();
			sibling.write(end_pid, (PageFile&)pf);
			//sibling.display(end_pid);
			leafnode.setNextNodePtr(end_pid);
			leafnode.write(cursor.pid,(PageFile&)pf);
			//leafnode.display(cursor.pid);
			//check all higher level nodes
			bool done = false;
			PageId current_pid;
			if(treeheight == 0){
				BTNonLeafNode root;
				root.initializeRoot(0, midKey, 1);
				root.write(2, (PageFile&) pf);
				//root.display(2);
				setRootPid(2);
				setTreeHeight(1);
			}
			else{
			while(pathTrace.size() != 0 && !done){
				current_pid = pathTrace.back();
				root.read(pathTrace.back(), (const PageFile&)pf);
				if(root.getKeyCount() < MaxNonLeafEntries){
					root.insert(midKey, end_pid);
					root.write(pathTrace.back(), (PageFile&)pf);
					//root.display(pathTrace.back());
					done = true;
				}
				else{
					BTNonLeafNode internal_node;
					//int new_key;
					root.insertAndSplit(midKey, end_pid, (BTNonLeafNode&)internal_node, (int&)midKey);
					root.write(pathTrace.back(), (PageFile&)pf);
					//root.display(pathTrace.back());
					end_pid = pf.endPid();
					internal_node.write(end_pid, (PageFile&)pf);
					//internal_node.display(end_pid);
					//end_pid = pathTrace.back();
					if(pathTrace.back() == rootpid){
						BTNonLeafNode new_root;
						new_root.initializeRoot(pathTrace.back(), midKey, end_pid);
						end_pid = pf.endPid();
						new_root.write(end_pid, (PageFile&) pf);
						//new_root.display(end_pid);
						setRootPid(end_pid);
						treeheight++;
						setTreeHeight(treeheight);
						//change rootpid and treeheight
						done = true;
					}
				}
				pathTrace.pop_back();
			}
			}
		}
		else{
			leafnode.insert(key, (const RecordId&)rid);
			leafnode.write(cursor.pid, (PageFile&)pf);
			//leafnode.display(cursor.pid);
		}
		pathTrace.clear();
		root.read(0, (const PageFile&)pf);//could verify that something was inserted
		root.setTotalKeys(total_keys + 1);
		root.write(0, (PageFile&)pf);
	}
    return 0;
}

/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	PageId next_pid;
	BTNonLeafNode searchnode;
	BTLeafNode leafnode;
	int value;
	if(pf.endPid() != 0){
		searchnode.read(0, pf);
		int rootpid = searchnode.getRootPid();
		int tree_height = searchnode.getTreeHeight();
		pathTrace.push_back(rootpid);
		searchnode.read(rootpid, (const PageFile&)pf);
		int eid;
		if(tree_height > 0){
			for(int i = 0; i < tree_height; i++){
				searchnode.locateChildPtr(searchKey, (PageId&) next_pid);
				pathTrace.push_back(next_pid);
				searchnode.read(next_pid, pf);
			}
		}
		else{
			next_pid = rootpid;
		}
		//run locate on that leaf node to get eid
		leafnode.read(next_pid, pf);
		value = leafnode.locate(searchKey, (int&)eid);
		cursor.eid = eid;
		//populate cursor
		cursor.pid = next_pid;
		pathTrace.pop_back();
	}
    return value;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	BTLeafNode leaf;
	leaf.read(cursor.pid, (const PageFile&) pf);
	leaf.readEntry(cursor.eid, key, rid);
	if(cursor.eid < leaf.getKeyCount() - 1){
		cursor.eid++; //make sure still in tree
	}
	else{
		cursor.pid = leaf.getNextNodePtr();
		cursor.eid = 0;
	}
    return 0;
}

RC BTreeIndex::setRootPid(PageId pid){
	BTNonLeafNode root;
	root.read(0, (const PageFile&)pf);
	root.setRootPid(pid);
	root.write(0, (PageFile&)pf);
	return 0;
}

RC BTreeIndex::setTreeHeight(int new_height){
	BTNonLeafNode root;
	root.read(0, (const PageFile&)pf);
	root.setTreeHeight(new_height);
	root.write(0, (PageFile&)pf);
	return 0;
}

PageId BTreeIndex::endPid() const{
	return pf.endPid();
}

int BTreeIndex::getTotalKeys(){
	BTNonLeafNode root;
	root.read(0, (const PageFile&)pf);
	return root.getTotalKeys();
}
