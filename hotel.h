struct item
{
    char name[100];
    double price;   //unit price
    int num;
    struct item *next;
};

struct customer
{
    char name[100],address[100],id_no[100];
    int date_in;
    struct item *receipt;
    struct room *p_room;
    struct customer *next;
};

struct room
{
    int no;
    int type; // 0 -- standard;  1 -- luxury;
    struct customer *p_customer; // NULL -- available;  otherwise -- occupied
    struct room *next;
};

typedef struct item Item;
typedef struct customer Customer;
typedef struct room Room;

void exit_program();
int checkNumber(char s[100]);
void invalidInput();
Customer *findCustomer(Customer *clist_h);

void check_in(Customer **p_clist_h,Room *rlist_h);
void check_out(Customer **p_clist_h);
void dine(Customer *c);
void phone_call(Customer *c);
void drink(Customer *c);
void enter_club(Customer *c);
void customers(Customer *clist_h);
void buildNewRoom(Room **p_rlist_h,int *p_room_no,int room_type);
void removeRoom(Room **p_rlist_h, int *p_room_no);
void findRoom(Customer *clist_h);
void findOccupier(Room *rlist_h);
