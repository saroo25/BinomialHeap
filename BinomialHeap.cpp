#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

template <class T> class BinomialTree{
    private:
        //valore del BinomialTree
        T value;
        // usiamo l'attributo degree per identificare di che ordine è
        int degree;
        //manteniamo un puntatore al padre, al figlio e al fratello
        BinomialTree<T>* parent;
        BinomialTree<T>* child;
        BinomialTree<T>* sibling;
    public:
        //metodo costruttore
        BinomialTree(T value){
            this->value=value;
            this->parent=NULL;
            this->child=NULL;
            this->sibling=NULL;
            this->degree=0;
        }
        
        ~BinomialTree(){
            delete child;
            delete sibling;
        }

        void cut(){
            this->parent=NULL;
            this->child=NULL;
            this->sibling=NULL;
            this->degree=0;
        }

        //metodi GETTER
        BinomialTree<T>* getParent(){return parent;}
        BinomialTree<T>* getChild(){return child;}
        BinomialTree<T>* getSibling(){return sibling;}
        T getValue(){return value;}
        int getDegree(){return degree;}

        //metodiSETTER
        void setParent(BinomialTree<T>* parent){
            this->parent=parent;
        }
        void setSibling(BinomialTree<T>* sibling){
            this->sibling=sibling;
        }
        void setChild(BinomialTree<T>* child){
            this->child=child;
        }
        void setValue(T value){
            this->value=value;
        }
        void increaseDegree(){degree++;}
        void decreaseDegree(){degree--;}
        
        /*
        si sceglie una funzione di tipo statico poichè si potrebbe chiamare il merge di due alberi senza passare dall'istanza di oggetto
        inoltre la procedura prende in input l'elemento con la priorità minore e l'elemento con la priorità maggiore
        */
        static BinomialTree<T>* mergeTrees(BinomialTree<T>* N1,BinomialTree<T>* N2){ 
                                        
            //CONTROLLARE SE GLI OGGETTI SONO NULL
            if(!N1) return N2;
            if(!N2) return N1;

            /*if(N1->getValue() > N2->getValue() ){//se il primo nodo è maggiore faccio uno swap( sostituire con una funzione magari)
                BinomialTree<T>* tmp=N1;//forse mettere la compare
                N1=N2;
                N2=tmp;
            }*/
            N1->setParent(N2);
            N1->setSibling(N2->getChild());
            N2->setChild(N1);
            N2->increaseDegree();
            return N2;
        }

        BinomialTree<T>* printTree(){//non voglio che la radice passi alle altre radici quindi faccio questo if
            if( parent==NULL){
                cout<<value<<" ";
                if(child) child->printTree();

            }
            else{
                
                    cout<<value<<" ";
                    if(child) child->printTree();
                    if(sibling) sibling->printTree();
            
            }

        }
        BinomialTree<T>* search(T key){
            //cout<<"sono nel nodo ";
            // cout<<value<<" fratello "<< sibling <<" figlio "<<child <<endl;
            if(this->value==key) return this;
            
            if(child!=NULL && child->search(key))   return child->search(key);
            
            if(sibling!=NULL && sibling->search(key)) return sibling->search(key);
            return NULL;
        }

};

template <class T> class BinomialHeap{
    private:
        BinomialTree<T>* head;

        void setHead(BinomialTree<T>* head){
            this->head=head;
        }

        BinomialTree<T>* getHighPriorityElementNode(){//la usiamo per estrarre il minimo
            //controlla se la testa è nulla, e quindi se l'heap è vuoto
            if(head){
                BinomialTree<T>* prev=NULL;
                BinomialTree<T>* temp=NULL;
                BinomialTree<T>* minval=head;
                BinomialTree<T>* prevmin=NULL;
                for(prev=head,temp=head->getSibling();temp!=NULL;prev=prev->getSibling(),temp=temp->getSibling())
                    if(compare(temp->getValue(),minval->getValue())<=0){
                        prevmin=prev;
                        minval=temp;
                    }
                if(prevmin)prevmin->setSibling(minval->getSibling());
                else head=minval->getSibling();
                return minval;
            }
            return NULL;
            
        }
        BinomialTree<T>* mergeHeap(BinomialTree<T>* bh1,BinomialTree<T>* bh2){
            BinomialTree<T>* it1=bh1;
            BinomialTree<T>* it2=bh2;
            BinomialTree<T>* merged;
            if(it1==NULL){//se uno dei due heap è nullo restituisco l'altro
                merged=it2;
                return merged;
            } 
            if(it2==NULL){
                merged=it1;
                return merged;
            } 

            if(it1->getDegree() < it2->getDegree()){//va scelto a che nodo inizializzare l'heap
                merged=it1;
                it1=it1->getSibling();
                
            }
            else{
                merged=it2;
                it2=it2->getSibling();
               
            }
            BinomialTree<T>* it3=merged;//merge dei nodi, secondo il grado, questo heap sarà ordinato secondo il grado.
            while(it1 && it2){
                if(it1->getDegree() < it2->getDegree()){
                    it3->setSibling(it1);
                    
                    it1=it1->getSibling();
                }
                else{
                    it3->setSibling(it2);
                    it2=it2->getSibling();
                }
                it3=it3->getSibling();
            }
            if(it1) it3->setSibling(it1);//se uno dei due heap non si è svuotato semplicemente si concatena ciò che resta all'heap appena creato
            if(it2) it3->setSibling(it2);
            return merged;
        }


        BinomialTree<T>* unionHeap(BinomialTree<T>* bh1,BinomialTree<T>* bh2){
            //BinomialHeap<T>* w=new BinomialHeap<T>;
            BinomialTree<T>* w ;
            w=(mergeHeap(bh1,bh2));//creo un nuovo heap che è l'unione dei due
            if(!w) return w;
            BinomialTree<T>* h=w;//inizializzo i 3 nodi
            BinomialTree<T>* prev=NULL;
            BinomialTree<T>* next=h->getSibling();
            
            while (next){
                if((h->getDegree()!=next->getDegree()) || (next->getSibling()!=NULL && next->getSibling()->getDegree()==h->getDegree())){//controlliamo che i nodi non abbiano grado uguale, o ce ne siano 3 di grado uguale
                    prev=h;//se due nodi consecutivi hanno grado diverso, o ci sono 3 nodi con grado uguale allora scorriamo avanti, ritardando il merge
                    h=next;
                    
                }
                else if(h->getDegree()==next->getDegree()){//se due alberi sono dello stesso grado e non c'è il terzo albero posso fare il merge
                    if(compare(h->getValue(), next->getValue() )<0){//semplici controlli per capire il nodo da sostituire
                        h->setSibling(next->getSibling());//h sarà il nuovo padre quindi prima del merge ci serve il suo vicino
                        BinomialTree<T>::mergeTrees(next,h);
                    }
                    else{
                        if(prev) prev->setSibling(next);//se il nodo precedente non è null va aggiornato il suo vicino
                        BinomialTree<T>::mergeTrees(h,next);
                        h=next;//next è il nodo padre poichè più piccolo,va quindi aggiornato h
                        if(!prev) w=h;//se h è il primo nodo dell'heap allora dobbiamo aggiornare affinchè punti al nuovo valore
                    }
                }
                next=h->getSibling();//aggiornamento del nodo
            }
            return w;
        }

        void swap(BinomialTree<T>* child,BinomialTree<T>* parent){//scegliamo questi nomi pochè un nodo si scambia solo con il padre
            BinomialTree<T>* tmp=new BinomialTree<T>(child->getValue());
            child->setValue(parent->getValue());
            parent->setValue(tmp->getValue());
            delete tmp;
        }
        
        BinomialTree<T>* reverseList(BinomialTree<T>* reverse){//poichè i figli di un nodo sono in ordine decreescente abbiao bisogno un list reverse
            BinomialTree<T>* prev=NULL;
            BinomialTree<T>* next=NULL;
            while(reverse){
                reverse->setParent(NULL);
                next=reverse->getSibling();
                reverse->setSibling(prev);
                prev=reverse;
                reverse=next;
            }
            return prev;
        }

        void supportDelete(BinomialTree<T>* tmp){
            BinomialTree<T>* parent=tmp->getParent();
            while(parent!= NULL){
                swap(tmp,parent);
                tmp=parent;
                parent=tmp->getParent();
            }

            BinomialTree<T>* prev=head;
            while(prev!=tmp && prev->getSibling()!=tmp){
                prev=prev->getSibling();
            }
            if(prev==tmp){
                head=head->getSibling();
            }
            else{
                prev->setSibling(tmp->getSibling());
            }
            BinomialTree<T>* w;
            w=(reverseList( tmp->getChild()) );//tagliare i collegamenti di tp e chiamare la sua delete
            tmp->cut();
            delete tmp;
            tmp=NULL;
            head=unionHeap(head,w);

        }


    public:
        BinomialHeap(){
            this->head=NULL;
         
        }

        virtual ~BinomialHeap(){
            delete head;
            head=NULL;
        }

        BinomialHeap(BinomialTree<T>* head){
            this->head=head;
       
        }

        BinomialTree<T>* getHead(){
            return head;
        }
        
        void printHead(){//usato per fixare , va eliminato
            cout<<head->getValue()<<endl;
        }

        void printHeap(){//stampa albero per albero
            cout<<"stampo l'heap"<<endl;

            for(BinomialTree<T>* tmp=head;tmp!=NULL;tmp=tmp->getSibling()){
                tmp->printTree();
                cout<<endl<<endl;
            }
            if(!head) cout<<"ops.. sembra che il tuo heap sia vuoto"<<endl;

            cout<<"fine stampa heap"<<endl;
        }


        virtual int compare(T X, T Y)=0;/* {//scegliamo di implementare la classe binomial heap generica come un min heap, affinchè essa possa funzionare autonomamente
                                       //se necessrio si può effettuare l'overriding di compare
            if(X<Y) return -1;
            else return 1;

        }*/


        T getHighPriorityElement(){
            //controlla se la testa è nulla, e quindi se l'heap è vuoto
            if(head){
                T minval=head->getValue();
                for(BinomialTree<T>* temp=head->getSibling();temp!=NULL;temp=temp->getSibling())
                    if(compare(temp->getValue(),minval)<=0){
                        minval=temp->getValue();
                    }
                return minval;
            }
        }

        int insetElement(T value){

            BinomialTree<T>* Vn=new BinomialTree<T>(value);//creiamo un nuovo heap che contiene solo il nuovo nodo da inserire
            head=unionHeap(head,Vn);//facciamo l'unione dei due heap

            return 1;//per convenzione se va a buon fine l'inserimento torniamo 1
        }


       

        BinomialTree<T>* extractHighPriorityelement(){
            BinomialTree<T>* minimum=this->getHighPriorityElementNode();//questa procedura scardina il nodo minimo dalla lista delle radici e lo ritorna

            BinomialTree<T>* reverse= reverseList(minimum->getChild());

            /*BinomialTree<T>* w=new BinomialTree<T>();
            w=reverse;*/

            head=unionHeap(head,reverse);
            return minimum;//si lascia all'utente che utilizzerà la classe il compito di deallocare la memoria, poichè sta estraendo l'elemento e non lo sta eliminando

            

        }

        BinomialTree<T>* search(T key){
            if(head) return  (head->search(key));
            else return NULL;
        }


        void increasePriority(T oldkey,T newkey){
            BinomialTree<T>* tmp=this->search(oldkey);
            if((compare(newkey,oldkey)<0) && tmp){
                //BinomialTree<T>* swap=NULL;
                BinomialTree<T>* parent= tmp->getParent();
                tmp->setValue(newkey);
                while(parent!=NULL && tmp!=NULL && compare(tmp->getValue(),parent->getValue() ) <0){
                    
                    swap(tmp,parent);//dopo questo scambio tmp punterà al nodo padre e non al nodo figlio
                    parent=parent->getParent();
                    tmp=tmp->getParent();
                    
                }

            }

        }

        void deleteElement(T key){
            BinomialTree<T>* tmp=this->search(key);
            if(tmp){//se l'elemento è presente possiamo cancellarlo
                supportDelete(tmp);
            }
        }

};

template <class T> class MaxBinomialHeap : public BinomialHeap<T>{
    public: 
    MaxBinomialHeap():BinomialHeap<T>() {
    }
    int compare(T X, T Y){
                if(X>Y) return -1;
                else return 1;
    }
};

template <class T> class MinBinomialHeap : public  BinomialHeap<T>{
    public: 
    MinBinomialHeap() :BinomialHeap<T>(){

    }
    int compare(T X, T Y){
                if(X<Y) return -1;
                else return 1;
    }
};


/*Procedure utilizzate per la generazione di testcase e controlli*/

template <class H> void initRandomArray(H *A,int len){
    srand(time(NULL));
    for(int i=0; i<len;i++) 
        A[i]=(rand()%1000)+1;
}

template <class H> void fillHeap(H *A,int len, BinomialHeap<H>* X){
    for(int i=0;i<len;i++)
        X->insetElement(A[i]);
}

template <class H> void orderArrayUsingHeap(H *A,int len,BinomialHeap<H>* X){//si crea un array ordinato con le estrazioni dall'heap
    for(int i=0;i<len;i++)
        A[i]=X->extractHighPriorityelement()->getValue();
}


template <class H> bool checkOrder(H *V,int len){//se l'array è ordinato allora le estrazioni hanno avuto tutte successo
    for(int i=0;i<len-1;i++){
        if(V[i]>V[i+1]){
            return false;
        }
    }
    return true;
}
template <class H> bool checkInverseOrder(H *V,int len){//se l'array è ordinato allora le estrazioni hanno avuto tutte successo
    for(int i=0;i<len-1;i++){
        if(V[i]<V[i+1]){
            return false;
        }
    }
    return true;
}





int main(){
    
    BinomialHeap<int>* ciccio=new MinBinomialHeap<int>();
    ciccio->insetElement(20);
    ciccio->insetElement(77);
    ciccio->insetElement(70);
    ciccio->insetElement(12);
    ciccio->insetElement(43);
    ciccio->insetElement(9);
    ciccio->insetElement(94);
    ciccio->insetElement(122);
    ciccio->insetElement(345);
    ciccio->insetElement(666);
    ciccio->insetElement(400);
    ciccio->insetElement(89);
    ciccio->printHeap();
    cout<<ciccio->getHighPriorityElement()<<endl;
    ciccio->extractHighPriorityelement();
    ciccio->printHeap();
    ciccio->increasePriority(43,11);
    ciccio->printHeap();
    ciccio->deleteElement(666);
    ciccio->deleteElement(20);
    ciccio->deleteElement(77);
    ciccio->deleteElement(70);
    ciccio->deleteElement(12);
    ciccio->deleteElement(43);
    ciccio->deleteElement(9);
    ciccio->deleteElement(94);
    ciccio->deleteElement(122);
    ciccio->deleteElement(345);
    ciccio->deleteElement(666);
    ciccio->deleteElement(400);
    ciccio->deleteElement(89);
    ciccio->deleteElement(11);

    ciccio->printHeap();
    delete ciccio;
    ciccio=NULL;


    ciccio = new MinBinomialHeap<int>();//generazione di task casuali per verificare che l'heap funzioni e non perda elementi o estragga male
    int task=10000;
    int len=1000;
    int *A=new int[len];
    bool  isordered=true;//se questa variabile rimane true allora gli array saranno tutti ordinati
    for(int i=0;i<task;i++){
        initRandomArray<int>(A,len);
        fillHeap<int>(A,len,ciccio);
        orderArrayUsingHeap<int>(A,len,ciccio);
        if(!checkOrder<int>(A,len)&& isordered) isordered=false;
    }
    cout<<isordered<<endl;

    delete ciccio;//generazione di task casuali per verificare che l'heap funzioni e non perda elementi o estragga male
    ciccio=NULL;
    ciccio = new MaxBinomialHeap<int>();
    isordered=true;

    for(int i=0;i<task;i++){
        initRandomArray<int>(A,len);
        fillHeap<int>(A,len,ciccio);
        orderArrayUsingHeap<int>(A,len,ciccio);
        if(!checkInverseOrder<int>(A,len)&& isordered) isordered=false;
    }

    cout<<isordered<<endl;

    return 0;
}
