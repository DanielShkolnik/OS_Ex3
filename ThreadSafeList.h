#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw

using namespace std;
pthread_mutex_t atomicLock;

template <typename T>
class List 
{
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;
        pthread_mutex_t nodeLock;
        // TODO: Add your methods and data members
        //Constructor
        Node(T data):data(data),next(nullptr), prev(nullptr){
            pthread_mutex_init(&nodeLock, NULL);
        }
        //Copy Constructor
        Node(const Node& node) = delete;
        //Operator= Constructor
        Node& operator=(const Node& node) = delete;
        ~Node(){
            pthread_mutex_destroy(&nodeLock);
        }
    };
private:
    Node* head;
    unsigned int size;
    // TODO: Add your own methods and data members
    public:
        /**
         * Constructor
         */
        List():head(nullptr),size(0){
            pthread_mutex_init(&atomicLock, NULL);
        }
        /**
         * Destructor
         */
        ~List(){
            Node* current=this->head;
            Node* prev=current;
            while(current!= nullptr){
               current=current->next;
               delete prev;
            }
            pthread_mutex_destroy(&atomicLock);
        }

        /**
         * Insert new node to list while keeping the list ordered in an ascending order
         * If there is already a node has the same data as @param data then return false (without adding it again)
         * @param data the new data to be added to the list
         * @return true if a new node was added and false otherwise
         */
        bool insert(const T& data) {
			Node* current=this->head;
            Node* prev=current;
            bool isPrevEqualCurrent = true;
            pthread_mutex_lock(&atomicLock);
			if(current == nullptr){
			    this->head = new Node(data);
			    if(this->head== nullptr){
                    //std::cerr << "insert:failed" << endl;
                    pthread_mutex_unlock(&atomicLock);
                    return false;
			    }
                pthread_mutex_unlock(&atomicLock);
                __insert_test_hook();
			    return true;
			}
            pthread_mutex_unlock(&atomicLock);
            pthread_mutex_lock(&(current->nodeLock));
			while(current != nullptr && current->data<data){
                pthread_mutex_lock(&atomicLock);

			    if(current->data == data){
                    pthread_mutex_unlock(&(current->nodeLock));
                    if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
                    pthread_mutex_unlock(&atomicLock);
                    return false;
			    }
                pthread_mutex_unlock(&(current->nodeLock));
                if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
			    prev=current;
                current=current->next;
                if(isPrevEqualCurrent) isPrevEqualCurrent=false;
                if(current != nullptr) pthread_mutex_lock(&(current->nodeLock));
                pthread_mutex_lock(&(prev->nodeLock));

                pthread_mutex_unlock(&atomicLock);
			}
			Node* node=new Node(data);
            if(node== nullptr){
                //std::cerr << "insert:failed" << endl;
                return false;
            }
            pthread_mutex_lock(&atomicLock);
            node->next=current;
			if(!isPrevEqualCurrent){
                node->prev=prev;
                prev->next->prev=node;
                prev->next=node;
			}
			else{
                node->prev= nullptr;
                prev->next->prev=node;
			}
			this->size++;
            pthread_mutex_unlock(&atomicLock);
            __insert_test_hook();
            pthread_mutex_lock(&atomicLock);
            if(current!= nullptr) pthread_mutex_unlock(&(current->nodeLock));
            if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
            pthread_mutex_unlock(&atomicLock);
			return true;
        }

        /**
         * Remove the node that its data equals to @param value
         * @param value the data to lookup a node that has the same data to be removed
         * @return true if a matched node was found and removed and false otherwise
         */
        bool remove(const T& value) {

            Node* current = this->head;
            Node* prev = current;
            bool isPrevEqualCurrent = true;

            pthread_mutex_lock(&(current->nodeLock));
            while(current != nullptr && current->data<value){
                pthread_mutex_lock(&atomicLock);

                if(current->data == value){
                    if(!isPrevEqualCurrent) prev->next=current->next;
                    if(current->next != nullptr){
                        if(!isPrevEqualCurrent) current->next->prev = prev;
                        else current->next->prev= nullptr;
                    }
                    delete current;
                    this->size--;
                    pthread_mutex_unlock(&atomicLock);
                    __remove_test_hook();
                    pthread_mutex_lock(&atomicLock);
                    pthread_mutex_unlock(&(current->nodeLock));
                    if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
                    pthread_mutex_unlock(&atomicLock);
                    return true;
                }
                pthread_mutex_unlock(&(current->nodeLock));
                if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
                prev=current;
                current=current->next;
                if(isPrevEqualCurrent) isPrevEqualCurrent=false;
                if(current != nullptr) pthread_mutex_lock(&(current->nodeLock));
                pthread_mutex_lock(&(prev->nodeLock));

                pthread_mutex_unlock(&atomicLock);
            }
            Node* node=new Node(data);
            if(node== nullptr){
                std::cerr << "insert:failed" << endl;
                return false;
            }
            pthread_mutex_lock(&atomicLock);
            if(current!= nullptr) pthread_mutex_unlock(&(current->nodeLock));
            if(isPrevEqualCurrent != true) pthread_mutex_unlock(&(prev->nodeLock));
            pthread_mutex_unlock(&atomicLock);
            return false;
        }

        /**
         * Returns the current size of the list
         * @return current size of the list
         */
        unsigned int getSize() {
			//TODO: add your implementation
			return this->size;
        }

		// Don't remove
        void print() {
          Node* temp = head;
          if (temp == NULL)
          {
            cout << "";
          }
          else if (temp->next == NULL)
          {
            cout << temp->data;
          }
          else
          {
            while (temp != NULL)
            {
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