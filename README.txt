harelka

Harel Kahani 311597777

Exercise 1 – String Pattern Matching Algorithm

Program files:
slist.c - file that implementaion a Linked list and it's Actions.
pattern_matching - file that implementaion string searching, creating DFA that representing the patterns, with that searching in the text.
    	
slist.c function::
    slist_init - initialize a new list.
	slist_destroy - de-allocate the memory hold by the list.
	slist_append - add node in the end of the list.
	slist_prepend - add node in the start of the list.
	slist_pop_first - remove the first node in the list and return his data.
	slist_append_list - merging 2 list, when are list2 insert in the end of list1.
private function:
	init_node - creating new node with desired data and ptr for next dest.

----------
pattern_matching functions:
	pm_init - initializes the fsm parameters.
	pm_goto_get - return the _transitions state.
	pm_goto_set - connect edge between from_state to to_state and the symbol as the edge.
	pm_addstring - add new pattern into the pm that was created.
	pm_makeFSM - setting up the failrue transitions in the fsm.
	pm_fsm_search - 
	pm_destroy - de-allocate the memory hold by the fsm.
private functions:
	void queue_append - adding all states of pm into a queue.
    void destroy_state- destroy each state in pm.



