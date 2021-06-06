#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>


namespace ariel{
    const unsigned int inorder_flag =  1;
    const unsigned int preorder_flag =  2;
    const unsigned int postorder_flag =  3;

    template<typename T> class BinaryTree{
        private:
            struct Node{
                T value;
                Node* left;
                Node* right;
                Node(T& v):value(v),left(nullptr),right(nullptr) {}
            };
            Node* head;
        public:
            BinaryTree():head(nullptr){}
            ~BinaryTree(){
                delet_tree(head);
            };
            BinaryTree& add_root(T root);
            BinaryTree& add_left(T exsist,T add);
            BinaryTree& add_right(T exsist,T add);
            Node* find_node(Node* root,T val){
                if(!root) {return nullptr;}
                if(root->value == val){return root;}
                Node* result = find_node(root->left,val);
                if(result){return result;}
                return find_node(root->right,val);
            }
            void delet_tree(Node* root){
                if(!root){return;}
                delet_tree(root->left);
                delet_tree(root->right);
                delete root;
                root = nullptr;
            }
            BinaryTree(const BinaryTree & other){
                head = new Node(other.head->value);
                deep_copy(*head,*other.head);
            }
            void deep_copy(Node& root, Node& other_root){
                if(other_root.left) {
                root.left = new Node(other_root.left->value);
                deep_copy(*root.left,*other_root.left);
                }
                if(other_root.right) {
                root.right = new Node(other_root.right->value);
                deep_copy(*root.right,*other_root.right);
                }
            }

            BinaryTree& operator=(const BinaryTree& other) {
                if (this == &other) {return *this;}
                //delet_tree(head);
                delete head;
                head = new Node(other.head->value);
                deep_copy(*head, *other.head);
                return *this;
    }

            BinaryTree& operator = (BinaryTree&& other)noexcept{
                head = other->head;
                other->head = nullptr;
                return *this;
            }

            BinaryTree(BinaryTree&& other)noexcept{
                head = other.head;
                other.head = nullptr;
            }
            friend std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& b){
                return out;
            }
    
            class iterator{
                private:
		            Node* pointer_to_current_node;
                    std::queue<Node*> tree_in_queue;

                public:  
                    iterator(Node* ptr):pointer_to_current_node(ptr){}  
                    iterator(unsigned int flag,Node* ptr): pointer_to_current_node(ptr){
                        if(pointer_to_current_node != nullptr){
                            if(flag == inorder_flag){Inorder(pointer_to_current_node);}
                            if(flag == preorder_flag){Preoder(pointer_to_current_node);}
                            if(flag == postorder_flag){Postorder(pointer_to_current_node);}
                            tree_in_queue.push(nullptr);
                        }
   
                    }
                    void Inorder(Node* root);
                    void Preoder(Node* root);
                    void Postorder(Node* root);
                    T& operator*() const {return pointer_to_current_node->value;}
		            T* operator->() const {return &(pointer_to_current_node->value);}
		            iterator& operator++() {
                        tree_in_queue.pop();
			            pointer_to_current_node = tree_in_queue.front();
			            return *this;
		            }
		            iterator operator++(int) {
			            iterator tmp = *this;
                        tree_in_queue.pop();
			            pointer_to_current_node = tree_in_queue.front();
			            return tmp;
		            }

		            bool operator==(const iterator& rhs) const {return pointer_to_current_node == rhs.pointer_to_current_node;}
		            bool operator!=(const iterator& rhs) const {return pointer_to_current_node != rhs.pointer_to_current_node;}
            };
 
            iterator begin_inorder(){return iterator(inorder_flag,head);}
            iterator end_inorder(){return iterator(nullptr);}
            iterator begin_preorder(){return iterator(preorder_flag,head);}
            iterator end_preorder(){return iterator(nullptr);}
            iterator begin_postorder(){return iterator(postorder_flag,head);}
            iterator end_postorder(){return iterator(nullptr);}
            iterator begin(){return iterator(inorder_flag,head);}
            iterator end(){return iterator(nullptr);}

    };
    template<typename T>
    BinaryTree<T>& BinaryTree<T>::add_root(const T root){
        if(!head){
            head = new Node(root);
            return *this;
        }
        head->value = root;
         return *this;
    }

    template<typename T>
    BinaryTree<T>& BinaryTree<T>::add_left(T exsist, T add){
        if(!head){throw std::invalid_argument{"Tree is empty"};}
        Node* node = find_node(head,exsist);
        if(!node){throw std::invalid_argument{"That node don't exsist"};}
        if(!node->left){
            node->left= new Node(add);
            return *this;
        }
        node->left->value = add;
        return *this;
    }

    template<typename T>
    BinaryTree<T>& BinaryTree<T>::add_right(T exsist, T add){
        if(!head){throw std::invalid_argument{"Tree is empty"};}
        Node* node = find_node(head,exsist);
        if(!node){throw std::invalid_argument{"That node don't exsist"};}
        if(!node->right){
            node->right = new Node(add);
            return *this;
        }
        node->right->value = add;
        return *this;
    }

    template<typename T>
    void BinaryTree<T>::iterator::Inorder(Node* root){
        std::stack<Node*> s;
        Node *curr = root;
        while (curr != nullptr || !s.empty()){
            while (curr !=  nullptr){
                s.push(curr);
                curr = curr->left;
            }
            curr = s.top();
            s.pop();
            tree_in_queue.push(curr);
            curr = curr->right;
        }
        pointer_to_current_node = tree_in_queue.front();
    }

    template<typename T>
    void BinaryTree<T>::iterator::Preoder(Node* root){
        if (root == nullptr){
            return;
        }
        std::stack<Node*> nodeStack;
        nodeStack.push(root);

        while (!nodeStack.empty()) {
            struct Node* node = nodeStack.top();
            tree_in_queue.push(node);
            nodeStack.pop();
 
            if (node->right){
                nodeStack.push(node->right);
            }
            if (node->left){
                nodeStack.push(node->left);
            }
        }
        pointer_to_current_node = tree_in_queue.front();
    }

    template<typename T>
    void BinaryTree<T>::iterator::Postorder(Node* root){
        if (root == nullptr){
            return;
        }
        std::stack<Node*> s1 ;
        std::stack<Node*> s2;
        s1.push(root);
        Node* node = nullptr;
  
        while (!s1.empty()) {
            node = s1.top();
            s1.pop();
            s2.push(node);
            if (node->left){
                s1.push(node->left);
            }
            if (node->right){
                s1.push(node->right);
            }
        }
        while (!s2.empty()) {
            node = s2.top();
            s2.pop();
            tree_in_queue.push(node);
        }
        pointer_to_current_node = tree_in_queue.front();
    }

}