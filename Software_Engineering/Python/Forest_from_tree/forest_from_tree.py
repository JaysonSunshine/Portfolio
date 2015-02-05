import math

def nodeCount(nodes, edges, b, amount):
    nodes[edges[b]] += amount
    if edges[b] in edges:
        nodeCount(nodes, edges, edges[b], amount)

def prune(nodes, edges, size, count, initial):
    if size < 2:
        return count
    dict = {k:v for (k,v) in nodes.iteritems() if v == size}
    dict = filter(lambda k: k in edges, dict)
    if len(dict) > 0:
        for elem in dict:
            count += 1
            nodeCount(nodes,edges,elem,size * -1)
            del edges[elem]
    count += prune(nodes, edges, size - 2, 0, initial)
    return count
     
#this function will turn a tree of even nodes into a forest of maximum number of 
#trees, all with even nodes, by pruning as many edges as possible from the
#original tree  
def main():
    node, edge = map(int, raw_input().split())
    if node % 2 == 0:
        edges = {}
        nodes = {}
        for i in range(1, node + 1):
            nodes[i] = 1
        for _ in range(0, edge):
            a, b = map(int,raw_input().split())
            edges[a] = b
            nodes[b] += 1
            if b in edges:
                nodeCount(nodes, edges, b, 1)
        count = prune(nodes, edges, node - 2, 0, i)
        print count
    else:
        print "Initial tree must contain even number of nodes"

if __name__ == "__main__":
    main()
