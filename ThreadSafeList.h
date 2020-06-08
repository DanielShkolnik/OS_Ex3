#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw

using namespace std;
pthread_mutex_t atomicLock;

template <typename T>
class List {
    class Node {
    public:
        T data;
        Node *next;
        Node *prev;
        pthread_mutex_t nodeLock;

        //Constructor
        Node(T data) : data(data), next(nullptr), prev(nullptr){
            pthread_mutex_init(&nodeLock, NULL);
        }

        //Copy Constructor
        Node(const Node &node) = delete;

        //Operator= Constructor
        Node &operator=(const Node &node) = delete;

        //Destructor
        ~Node() {
            pthread_mutex_destroy(&nodeLock);
        }

    };

private:
    Node *head;
    unsigned int size;
public:
    /**
     * Constructor
     */
    List() : size(0) {
        this->head = new Node(0);
        if(this->head == nullptr){
            std::cerr << "insert:failed" << endl;
            exit(-1);
        }
        pthread_mutex_init(&atomicLock, NULL);
    }

    /**
     * Destructor
     */
    ~List() {
        Node *current = this->head;
        Node *prev = current;
        while (current != nullptr) {
            current = current->next;
            delete prev;
            prev = current;
        }
        pthread_mutex_destroy(&atomicLock);
    }

    /**
     * Insert new node to list while keeping the list ordered in an ascending order
     * If there is already a node has the same data as @param data then return false (without adding it again)
     * @param data the new data to be added to the list
     * @return true if a new node was added and false otherwise
     */
    bool insert(const T &data){
        pthread_mutex_lock(&(this->head->nodeLock));
        if(this->head->next!= nullptr) pthread_mutex_lock(&(this->head->next->nodeLock));
        Node *current = this->head->next;
        Node *prev = this->head;

        while (current != nullptr && current->data <= data) {
            if (current->data == data) {
                if (current != nullptr) pthread_mutex_unlock(&(current->nodeLock));
                pthread_mutex_unlock(&(prev->nodeLock));
                return false;
            }
            if(current->next != nullptr) pthread_mutex_lock(&(current->next->nodeLock));
            pthread_mutex_unlock(&(prev->nodeLock));
            prev = current;
            current = current->next;
        }
        Node *node = new Node(data);
        if (node == nullptr) {
            if (current != nullptr) pthread_mutex_unlock(&(current->nodeLock));
            pthread_mutex_unlock(&(prev->nodeLock));
            return false;
        }
        //if node is last
        if (current == nullptr) {
            node->next = nullptr;
            node->prev = prev;
            prev->next = node;
        }
        //if node isn't last
        else {
            node->next = current;
            node->prev = prev;
            prev->next = node;
            current->prev = node;
        }
        pthread_mutex_lock(&atomicLock);
        this->size++;
        pthread_mutex_unlock(&atomicLock);
        __insert_test_hook();
        if (current != nullptr) pthread_mutex_unlock(&(current->nodeLock));
        pthread_mutex_unlock(&(prev->nodeLock));
        return true;
    }

    /**
     * Remove the node that its data equals to @param value
     * @param value the data to lookup a node that has the same data to be removed
     * @return true if a matched node was found and removed and false otherwise
     */

    bool remove(const T &value) {
        pthread_mutex_lock(&(this->head->nodeLock));
        if(this->head->next!= nullptr) pthread_mutex_lock(&(this->head->next->nodeLock));
        Node *current = this->head->next;
        Node *prev = this->head;
        while (current != nullptr && current->data <= value) {
            if (current->data == value) {
                //if node is last
                if (current->next == nullptr) {
                    prev->next = nullptr;
                }
                //if node isn't last
                else {
                    current->next->prev = prev;
                    prev->next = current->next;
                }
                pthread_mutex_unlock(&(current->nodeLock));
                delete current;
                pthread_mutex_lock(&atomicLock);
                this->size--;
                pthread_mutex_unlock(&atomicLock);
                __remove_test_hook();
                pthread_mutex_unlock(&(prev->nodeLock));
                return true;
            }
            if(current->next != nullptr) pthread_mutex_lock(&(current->next->nodeLock));
            pthread_mutex_unlock(&(prev->nodeLock));
            prev = current;
            current = current->next;
        }
        if (current != nullptr) pthread_mutex_unlock(&(current->nodeLock));
        pthread_mutex_unlock(&(prev->nodeLock));
        return false;
    }

    /**
     * Returns the current size of the list
     * @return current size of the list
     */
    unsigned int getSize() {
        return this->size;
    }

    // Don't remove
    void print() {
        Node *temp = this->head->next;
        if (temp == NULL) {
            cout << "";
        } else if (temp->next == NULL) {
            cout << temp->data;
        } else {
            while (temp != NULL) {
                cout << right << setw(3) << temp->data;
                temp = temp->next;
                cout << " ";
            }
        }
        cout << endl;
    }

    // Don't remove
    virtual void __insert_test_hook() {}

    // Don't remove
    virtual void __remove_test_hook() {}

};

#endif //THREAD_SAFE_LIST_H_