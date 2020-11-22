all: test_naive test_tree test_chtbl

test_naive: test_naive.c naive.c DictNode.c
	gcc test_naive.c naive.c DictNode.c -o test_naive
test_tree: test_tree.c RBTree.c DictNode.c
	gcc test_tree.c RBTree.c DictNode.c -o test_tree
test_chtbl: test_chtbl.c chtbl.c list.c DictNode.c
	gcc test_chtbl.c chtbl.c list.c DictNode.c -o test_chtbl

clean:
	rm -f *.o test_naive test_tree test_chtbl