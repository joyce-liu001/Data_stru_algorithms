
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "Map.h"
#include "Queue.h"

#define MAX_PEOPLE 128
#define NOT_VISITED -1
#define VISITED 0
#define MAX_REC 20



struct fb {
    int   numPeople;

    char *names[MAX_PEOPLE]; // the id of a person is simply the index
                             // that contains their name in this array
    
    Map   nameToId; // maps names to ids
                    // question to think about: why do we have this when
                    // the names array already provides this information?

    bool  friends[MAX_PEOPLE][MAX_PEOPLE];
};



static char *myStrdup(char *s);
static int   nameToId(Fb fb, char *name);
static void printrecom(Fb fb, int friend, char *name, int id);


////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb   FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->nameToId = MapNew();

    for (int i = 0; i < MAX_PEOPLE; i++) {
        for (int j = 0; j < MAX_PEOPLE; j++) {
            fb->friends[i][j] = false;
        }
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }

    MapFree(fb->nameToId);
    free(fb);
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == MAX_PEOPLE) {
        fprintf(stderr, "error: could not add more people\n");
        exit(EXIT_FAILURE);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = myStrdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (!fb->friends[id1][id2]) {
        fb->friends[id1][id2] = true;
        fb->friends[id2][id1] = true;
        return true;
    } else {
        return false;
    }
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (fb->friends[id1][id2]) {
        fb->friends[id1][id2] = false;
        fb->friends[id2][id1] = false;
        return true;
    } else {
        return false;
    }
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return fb->friends[id1][id2];
}

List FbGetFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    List l = ListNew();
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            ListAppend(l, fb->names[id2]);
        }
    }
    return l;
}

int  FbNumFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    int numFriends = 0;
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            numFriends++;
        }
    }
    return numFriends;
}

////////////////////////////////////////////////////////////////////////
// Your tasks

List FbMutualFriends(Fb fb, char *name1, char *name2) {
    List list =  ListNew();
    int friend = 0;
    while (fb->names[friend] != NULL) {
       
        if (FbIsFriend(fb, name1, fb->names[friend]) == true && 
        FbIsFriend(fb, name2, fb->names[friend]) == true) {
            // The person is both name1 and name2 friend.
            ListAppend(list, fb->names[friend]);
        }
        friend++;
    }

    return list;
}

void FbFriendRecs1(Fb fb, char *name) {
    printf("%s's friend recommendations\n", name);
    
    int friend = FbNumFriends(fb, name); 
    int id = nameToId(fb, name);
    printrecom(fb, friend, name, id);
}

static void printrecom(Fb fb, int friend, char *name, int id) {
    if (friend == 0) { // no firends means no mutual friends.
        return;
    }
    
    int i = 0;
    while (i < fb->numPeople) {
        if (i == id) { // same person
            i++;
        }
        List mutual = FbMutualFriends(fb, name, fb->names[i]);
        int size = ListSize(mutual);
        if (size > 0 && size == friend && FbIsFriend(fb, name, fb->names[i]) == false) {
            // This person (fb->names[i]) is not name's friend and has matual 
            // friends with name. And he has most mutual friends. he should be recommended. 
            printf("\t%-20s%4d mutual friends\n", fb->names[i], size);
        }
        i++;
        ListFree(mutual);
    }

    printrecom(fb, friend - 1, name, id);
}

void FbFriendRecs2(Fb fb, char *name) {
    printf("%s's friend recommendations\n", name);
    
    // Create array to count visitied.
    int visited[MAX_PEOPLE];
    int i = 0;
    while (i < fb->numPeople) {
        // initialise visited.
        visited[i] = NOT_VISITED;
        i++;
    }
    int id = nameToId(fb, name);
    visited[id] = VISITED; // source of graph
    
    // create a queue.
    Queue q = QueueNew();
    
    // enqueue src into queue Q
    QueueEnqueue(q, id);

    int recom = id;
    int number = 0;
    while (!QueueIsEmpty(q) && number <= MAX_REC) { // until Q is empty, and recommend less than 20 friends
        i = 0;
        while (i < fb->numPeople) { // for each friends of name with visited[j]= NOT_VISITED
            if (FbIsFriend(fb, fb->names[recom], fb->names[i]) == true && i != recom 
            && visited[i]== NOT_VISITED) {
                QueueEnqueue(q, i);
                visited[i] = VISITED;
            }
            i++;
        }
        
        recom = QueueDequeue(q);
        // when they are not friend and not same person
        if (FbIsFriend(fb, name, fb->names[recom]) == false && recom != id) { 
            // recommend this friend.
            printf("\t%s\n", fb->names[recom]);
            number++;
        } 
    }

    QueueFree(q);
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}

