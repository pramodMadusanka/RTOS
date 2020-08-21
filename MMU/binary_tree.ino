#define COUNT 10

struct node{
  int start = 0;
  int size = 0;
  int max_left = 0;
  int max_right = 0;
  struct node* left_child = NULL;
  struct node* right_child = NULL;
  struct node* parent = NULL;
};

struct node* root_ = NULL;

struct node* createNode(int start, int size){
  struct node* tmp = (struct node*)malloc(sizeof(struct node));
  tmp->start = start;
  tmp->size = size;
  tmp->max_left = 0;
  tmp->max_right = 0;
  tmp->left_child = NULL;
  tmp->right_child = NULL;
  tmp->parent = NULL;

  return tmp;
}

void removeNode(struct node* node_to_remove){
  struct node* left_most_child;
  node_to_remove->right_child->parent = node_to_remove->parent;
  node_to_remove->parent->right_child = node_to_remove->right_child;
  for(struct node* tmp=node_to_remove->right_child; tmp!=NULL; tmp=tmp->left_child){
    left_most_child = tmp;
  }
  node_to_remove->left_child->parent = left_most_child;
  left_most_child->left_child = node_to_remove->left_child;
}

void coalesce(int start, int size, struct node** root){
  struct node* closest_to_root;
  if((*root)->start + (*root)->size == start){
    (*root)->size +=size;
    for(struct node* tmp=(*root)->right_child; tmp!=NULL; tmp=tmp->left_child){
      closest_to_root = tmp;
    }
    if(closest_to_root->start == (*root)->start + (*root)->size){
      (*root)->size += closest_to_root->size;
      removeNode(closest_to_root);
    }
  }else if(start + size == (*root)->start){
    (*root)->start = start;
    (*root)->size += size;
    for(struct node* tmp=(*root)->left_child; tmp!=NULL; tmp=tmp->right_child){
      closest_to_root = tmp;
    }
    if(closest_to_root->start + closest_to_root->size == (*root)->start){
      (*root)->start = closest_to_root->start;
      (*root)->size += closest_to_root->size;
      removeNode(closest_to_root);
    }
  }
}

void insertNode(int start, int size, struct node** root){
  if(*root == NULL){
    *root = createNode(start, size);
  }else{
    if(((*root)->start + (*root)->size == start)||(start + size == (*root)->start)){
      coalesce(start, size, root);
    }else{
      if(start < (*root)->start){
        if((*root)->left_child == NULL){
          (*root)->left_child = createNode(start, size);
        }else{
          insertNode(start, size, &((*root)->left_child));
        }
      }else{
        if((*root)->right_child == NULL){
          (*root)->right_child = createNode(start, size);
        }else{
          insertNode(start, size, &((*root)->right_child));
        }
      }
    }
  }
}

void print2DUtil(struct node* root, int space){
    if (root == NULL) 
        return; 
  
    space += COUNT; 
  
    print2DUtil(root->right_child, space); 

    Serial.print("\n"); 
    for (int i = COUNT; i < space; i++) 
        Serial.print(" ");  
    Serial.println(root->start); 
  
    print2DUtil(root->left_child, space); 
}

void print2D(struct node *root) { 
   print2DUtil(root, 0); 
} 

void setup() {
  Serial.begin(9600);
}

void loop() {
  insertNode(1582, 10, &root_);
  insertNode(1112, 10, &root_);
  insertNode(2720, 10, &root_);
  insertNode(973, 10, &root_);
  insertNode(1271, 10, &root_);
  insertNode(1981, 10, &root_);
  insertNode(4812, 10, &root_);
  insertNode(1592, 10, &root_);
  insertNode(1572, 10, &root_);
  insertNode(2730, 10, &root_);
  insertNode(2710, 10, &root_);

  print2D(root_);

  while(1);
}
