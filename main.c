#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hotel.h"


int main()
{
    char hotel_name[30] = "Aha"; // the name of our hotal is "Aha"
    char hotel_address[100] = "Idiot Street No.59"; // the address of our hotel is "Idiot Street No.59"
    int room_no = 0; // the maximum of room is 100
    Room *rlist_h = 0; // the head of room list
    Customer *clist_h = 0; // the head of customer list
    
    int i; // first build 5 standard rooms and 5 luxury rooms
    for (i=0; i<5; i++)
    {
        buildNewRoom(&rlist_h,&room_no,0);
        buildNewRoom(&rlist_h,&room_no,1);
    }
    printf("Welcome to %s!\n\n",hotel_name);
    
    char input[100]; // be used to get the choice
    while (1)
    {
        printf("\nWhat sevice do you need?\n(Please enter the number of the sevice)\n");
        printf("1. Check-in\n");
        printf("2. Check-out\n");
        printf("3. Order some food\n");
        printf("4. Make a phone call\n");
        printf("5. Have a drink\n");
        printf("6. Enter the club\n");
        printf("9. Enter the management menu (if you are the manager)\n");
        printf("0. Exit the program\n");
        gets(input);
        
        int i;
        i = checkNumber(input); // transfer the input into number, if the input is not a number, give error
        if (i<0)
            continue;

        Customer *p;
        switch (i) // do different functions with different input
        {
            case 1:
                check_in(&clist_h,rlist_h);
                break;
            case 2:
                check_out(&clist_h);
                break;
            case 3:
                p = findCustomer(clist_h);
                if (p!=0)
                    dine(p);
                break;
            case 4:
                p = findCustomer(clist_h);
                if (p!=0)
                    phone_call(p);
                break;
            case 5:
                p = findCustomer(clist_h);
                if (p!=0)
                    drink(p);
                break;
            case 6:
                p = findCustomer(clist_h);
                if (p!=0)
                    enter_club(p);
                break;
             
            case 9: // for manager, you can do other functions such as buildNewRoom, findRoom, findOccupier
                printf("\nThis is the management menu.\n");
                while (1)
                {
                    printf("Please select the function:\n");
                    printf("1. Show customer list\n");
                    printf("2. Build a new room\n");
                    printf("3. Remove a room\n");
                    printf("4. Find the room occupied by the specified customer\n");
                    printf("5. Find the customer who is occupying the specified room\n");
                    printf("9. Return to the previous menu\n");
                    printf("0. Exit the program\n");
                    gets(input);
                    
                    int j;
                    j = checkNumber(input);
                    if (j<0)
                        continue;
                    
                    int flag = 1;
                    switch (j)
                    {
                        case 1:
                            customers(clist_h);
                            break;
                        case 2:
                            buildNewRoom(&rlist_h,&room_no,-1);
                            break;
                        case 3:
                            removeRoom(&rlist_h,&room_no);
                            break;
                        case 4:
                            findRoom(clist_h);
                            break;
                        case 5:
                            findOccupier(rlist_h);
                            break;
                        case 9:
                            flag = 0;
                            break;
                        case 0:
                            exit_program();
                            return 0;
                        default:
                            invalidInput();
                            break;
                    }
                    if (flag==0)
                        break;
                }
                break;
            
            case 0:
                exit_program();
                return 0;
            default:
                invalidInput();
                break;
        }
    }
    
    return 0;
}

int checkNumber(char s[100]) // transfer the input into a number, if the input is not a number, call a function "invalidInput", and return -1
{
    int flag,ans;
    flag = sscanf(s,"%d",&ans);
    //printf("\n\n%d\n\n",flag);
    if ( (flag<=0) || (ans<0)/*|| (strlen(s)!=1) */)
    {
        invalidInput();
        return -1;
    }
    return ans;

}

void invalidInput() // when the input is wrong, call this function and remind user to enter correct input
{
    printf("\nSorry, your input is invalid.\nPlease enter the correct instruction again.\n\n");
}

void exit_program() // end the program with a "Thank you"
{
    printf("\nThank you for using this program!\n");
}

Customer *findCustomer(Customer *clist_h) // to find if there is a customer by the name entered
{
    char input[100];
    printf("Please enter your name:\n");
    gets(input);
    Customer *p;
    p = clist_h;
    while (p!=0) // find the name in the customer list
    {
        if (strcmp(p->name,input)==0) {
            return p;
        }
        p = p->next;
    }
    printf("\nSorry, we can't find your name in customer list. Please check-in first.\n\n");
    return 0;
    
}

void check_in(Customer **p_clist_h, Room *rlist_h) // check in a customer and add his data into customer list
{
    int sremain = 0, lremain = 0; // number of standard rooms available;  number of luxury rooms available
    Room *p1, *p2[2];
    p1 = rlist_h;
    while (p1!=0) {
        if (p1->p_customer==0)
        {
            if (p1->type==0)
            {
                sremain++;
                p2[0] = p1;
            }
            else
            {
                lremain++;
                p2[1] = p1;
            }
        }
        p1 = p1->next;
    }
    
    if ((sremain==0)&&(lremain==0)) // if there is no room left, the manager should build new room
    {
        printf("\nSorry, there is no room available now.\n\n");
        return;
    }
    printf("\nThere are %d standard rooms available now.\n",sremain);
    printf("There are %d luxury rooms available now.\n",lremain);
    
    int room_type = -1;
    while (1) // choose the room type
    {
        printf("Which type of room do you prefer?\n");
        if (sremain>0)
            printf("1. Standard room (HK$700/day, 10%% room sevice fee）\n");
        if (lremain>0)
            printf("2. Luxury room   (HK$700/day, 10%% room sevice fee）\n");
        printf("9. Return to the previous menu\n");
        
        char input[100];
        int i;
        gets(input);
        i = checkNumber(input);
        if (i<0)
            continue;
        
        switch (i)
        {
            case 1:
                if (sremain==0)
                    invalidInput();
                else
                    room_type = 0;
                break;
            case 2:
                if (lremain==0)
                    invalidInput();
                else
                    room_type = 1;
                break;
            case 9:
                return;
            default:
                invalidInput();
                break;
        }
        if (room_type>=0)
            break;
    }
    
    Customer *c; // get the customer's information
    char input[100];
    c = (Customer *)malloc(sizeof(Customer));
    printf("Please enter your name:\n");
    gets(input);
    strcpy(c->name,input);
    printf("Please enter your address:\n");
    gets(input);
    strcpy(c->address, input);
    printf("Please enter your ID/passport number:\n"); // can be either numbers or alphabets
    gets(input);
    strcpy(c->id_no, input);
    while (1)
    {
        printf("Please enter the date today: (1-30)\nDay "); // ignore the month
        gets(input);
        int flag,ans;
        flag = sscanf(input,"%d",&ans);
        if ((flag>0)&&(ans>=1)&&(ans<=30))
        {
            c->date_in = ans;
            break;
        }
        invalidInput();
    }
    c->receipt = 0;
    c->next = 0;
    c->p_room = p2[room_type];
    c->next = *p_clist_h;
    *p_clist_h = c;
    
    p2[room_type]->p_customer = c;
    printf("\nWe have your room ready, %s.\n",c->name);
    printf("Your room number is %d.\n\n",c->p_room->no);
    printf("(Press Enter to return)\n");
    gets(input);
    
}

void check_out(Customer **p_clist_h) // check out a customer and give the detail of the receipt
{
    Customer *p1, *p4;
    char input[100];
    int days;

    p1 = findCustomer(*p_clist_h);
    if (p1==0)
        return;
    p1->p_room->p_customer = 0;
    
    while (1)
    {
        int flag,ans;
        printf("Please enter the date today: (2-31)\nDay "); // enter the leaving day
        gets(input);
        flag = sscanf(input,"%d",&ans);
        if ((flag>0)&&(ans>p1->date_in)&&(ans<=31))
        {
            days = ans - p1->date_in;
            break;
        }
        invalidInput();
    }
    
    Item *p2,*p3;
    double subtotal = 0;
    printf("\nThis is your receipt:\n"); // print the receipt with the unit price, number and total price
    printf("Item                Unit price   Num/Day   Total price\n");
    if (p1->p_room->type == 0) {
        printf("Standard Room  ");
    }else
    {
        printf("Luxury Room    ");
    }
    printf("     HK$ %6.1lf%10.d%14.1lf\n",700.0,days,700.0*days);
    printf("Room sevice fee     HK$ %6.1lf%10.d%14.1lf\n",70.0,days,70.0*days);
    subtotal += 700.0*days + 70.0*days;
    p2 = p1->receipt;
    while (p2!=0) {
        printf("%-20sHK$ %6.1lf%10.d%14.1lf\n",p2->name,p2->price,p2->num,(p2->price)*(p2->num));
        subtotal += (p2->price)*(p2->num);
        p3 = p2;
        p2 = p2->next;
        free(p3);
    }
    printf("Subtotal: %34.s%10.1lf\n","HK$",subtotal); // calculate the total fee of the customer
    printf("\nThank you for staying in our hotel!\n");
    
    if (*p_clist_h == p1)
    {
        *p_clist_h = p1->next;
    }
    else
    {
        p4 = *p_clist_h;
        while (p4->next != p1) {
            p4 = p4->next;
        }
        p4->next = p1->next;
    }
    free(p1);
    
    printf("(Press Enter to return)\n");
    gets(input);
    
}

void dine(Customer *c) // order the meal for a customer
{
    Item *p1;
    int set;
    char input[100];
    static char item_name[100] = "Meal";
    printf("\nOur restaurant provide one kind of meal (HK$50/set)\n");
    printf("How many sets do you want?\n(Please enter an integer）\n");
    gets(input);
    set = checkNumber(input);
    if (set<=0)
        return;
    printf("You have ordered %d sets of meals.\n",set);
    
    p1 = c->receipt; // add item into receipt
    while (p1!=0)
    {
        if (strcmp(p1->name,item_name)==0)
            break;
        p1 = p1->next;
    }
    if (p1==0)
    {
        p1 = (Item *) malloc(sizeof(Item));
        strcpy(p1->name,item_name);
        p1->price = 50.0;
        p1->num = 0;
        p1->next = c->receipt;
        c->receipt = p1;
    }
    p1->num += set;
    
}

void phone_call(Customer *c) // let the customer to have a phone call
{
    Item *p1;
    char input[100];
    int i = 0;
    static char item_name_1[100] = "Local phone call";
    static char item_name_2[100] = "Long distance call";
    int n = -1;
    if ((*c).p_room->type == 0) // if the customer is in a standard room, he can only make local phone call
    {
        printf("\nYou are in a standard room, so you can only make local phone call at a rate of HK $0.5 per minute.\n");
        while(n <= 0)
        {
            printf("\nHow long did you have the phone call?\n");
            gets(input);
            n = checkNumber(input);
            if ((n <= 0) && (n != -1))
            {
                invalidInput();
            }
        }
        p1 = c->receipt;
        while (p1 != 0) // add item into receipt
        {
            if (strcmp(p1->name,item_name_1) == 0)
                break;
            p1 = p1->next;
        }
        if (p1 == 0)
        {
            p1 = (Item *) malloc(sizeof(Item));
            strcpy(p1->name,item_name_1);
            p1->price = 0.5;
            p1->num = 0;
            p1->next = c->receipt;
            c->receipt = p1;
        }
        p1->num += n;
        printf("\nYou have made %d minutes %s.\n", n, item_name_1);
    }
    else if((*c).p_room->type == 1) // if the customer is in a luxury room, he can make either local phone call or long distance phone call
    {
        printf("\nYou are in a luxury room, so you can make local phone call at a rate of HK $0.5 per minute or make a long distance call at a rate of HK $5 per minute.\n");
        while ((i != 1) && (i != 2))
        {
            printf("\nPlease select your type of call:\n1. local call\n2. long distance call\n");
            gets(input);
            i = checkNumber(input);
            if ((i != 1) && (i != 2) && (i != -1))
            {
                invalidInput();
            }
        }
        while(n <= 0)
        {
            printf("\nHow long did you have the phone call?\n");
            gets(input);
            n = checkNumber(input);
            if ((n <= 0) && (n != -1))
            {
                invalidInput();
            }
        }
        if (i == 1)
        {
            p1 = c->receipt;
            while (p1 != 0) // add item into receipt
            {
                if (strcmp(p1->name,item_name_1) == 0)
                    break;
                p1 = p1->next;
            }
            if (p1 == 0)
            {
                p1 = (Item *) malloc(sizeof(Item));
                strcpy(p1->name,item_name_1);
                p1->price = 0.5;
                p1->num = 0;
                p1->next = c->receipt;
                c->receipt = p1;
            }
            p1->num += n;
            printf("\nYou have made %d minutes %s.\n", n, item_name_1);
        }
        else if (i == 2)
        {
            p1 = c->receipt;
            while (p1!=0) // add item into receipt
            {
                if (strcmp(p1->name,item_name_2)==0)
                    break;
                p1 = p1->next;
            }
            if (p1==0)
            {
                p1 = (Item *) malloc(sizeof(Item));
                strcpy(p1->name,item_name_2);
                p1->price = 5.0;
                p1->num = 0;
                p1->next = c->receipt;
                c->receipt = p1;
            }
            p1->num += n;
            printf("\nYou have made %d minutes %s.\n", n, item_name_2);
        }
    }
}

void drink(Customer *c) // order a drink for a customer in luxury room
{
    Item *p1;
    int n;
    char input[100];
    static char item_name[100] = "Wine";
    if (c->p_room->type == 0) // if the customer is not in luxury room, he cannot order the drink
    {
        printf("\nSorry, only the customer in luxury room can order the drink.\n\n");
        return;
    }
    
    printf("\nPlease input the number of wine: (HK$70/bottle)\n");
    gets(input);
    n= checkNumber(input);
    if (n<=0)
        return;
    printf("You have ordered %d bottles of wine.\n\n",n);
    
    p1 = c->receipt;
    while (p1!=0) // add item into receipt
    {
        if (strcmp(p1->name,item_name)==0)
            break;
        p1 = p1->next;
    }
    if (p1==0)
    {
        p1 = (Item *) malloc(sizeof(Item));
        strcpy(p1->name,item_name);
        p1->price = 70.0;
        p1->num = 0;
        p1->next = c->receipt;
        c->receipt = p1;
    }
    p1->num += n;
}

void enter_club(Customer *c) // let customer in luxury enter club
{
    Item *p;
    char input[100];
    static char item_name[100] = "Club entrance fee";
    if (c->p_room->type == 0) // if the customer is not in luxury room, he cannot enter the club
    {
        printf("\nSorry, only the customer in luxury room can enter the club.\n\n");
        return;
    }

    p = c->receipt;
    while (p!=0) // add item into receipt
    {
        if (strcmp(p->name, item_name)==0)
        {
            break;
        }
        p = p->next;
    }
    if(p == NULL)
    {
        p=(Item *)malloc(sizeof(Item));
        p->price=50.0;
        p->num=1;
        strcpy(p->name,item_name);
        p->next = c->receipt;
        c->receipt=p;
        printf("\nHK$ 50 has been charged as the club entrance fee.\n");
    }
    else // if the customer has entered club, it will not charge the fee again
    {
        printf("\nYou have paid the club entrance fee.\n");
    }
    printf("Please enter the club.\n");
    printf("(Press Enter to return)\n");
    gets(input);
}

void customers(Customer *clist_h) // list out the customer by alphabet order
{
    struct temp
    {
        char name[100];
        struct temp *next;
    };
    typedef struct temp Temp;
    
    if (clist_h == 0)
    {
        printf("\nNo customer now.\n\n");
        return;
    }
    
    Customer *p1;
    Temp *p2, *p3, *minpre, *head;
    char k[100];
    
    p1 = clist_h;
    head = 0;
    while (p1!=0)
    {
        p2 = (Temp *) malloc(sizeof(Temp));
        strcpy(p2->name, p1->name);
        p2->next = head;
        head = p2;
        p1 = p1->next;
    }
    p2 = (Temp *) malloc(sizeof(Temp));
    p2->next = head;
    head = p2;
    
    printf("\nCustomer list:\n");
    while (head->next != 0) // print out the list by using list
    {
        p2 = head->next;
        minpre = head;
        p3 = minpre;
        strcpy(k,p2->name);
        while (p2!=0)
        {
            if (strcmp(k,p2->name)>0)
            {
                minpre = p3;
                strcpy(k,p2->name);
            }
            p3 = p2;
            p2 = p2->next;
        }
        
        printf("%s\n",k);
        p2 = minpre->next;
        minpre->next = p2->next;
        free(p2);
    }
    printf("\n\n");
    free(head);
}

void buildNewRoom(Room **p_rlist_h,int *p_room_no,int room_type) // build new rooms if you are a manager
{
    char input[100];
    int i = room_type;
    if(*p_room_no<=100)
    {
        if (room_type==-1)
        {
            while (1)
            {
                printf("Please enter the room type (0 or 1):\n(0 -- standard room;  1 -- luxury room)\n");
                gets(input);
                i = checkNumber(input);
                if (i==-1)
                {
                    continue;
                }
                if ((i==0)||(i==1))
                    break;
                else
                    invalidInput();
                    continue;
            }
        }
        
        Room *p;
        int j,new_no;
        for (j=1; j<=*p_room_no+1; j++) // create new room in the roomlist
        {
            p = *p_rlist_h;
            int flag = 1;
            while (p!=0)
            {
                
                if (p->no == j) {
                    flag = 0;
                    break;
                }
                p = p->next;
            }
            if (flag)
            {
                new_no = j;
                break;
            }
        }
        p=(Room *)malloc(sizeof(Room));
        p->type=i;
        *p_room_no = *p_room_no+1;
        p->no=new_no;
        
        if (room_type==-1)
        {
            if(p->type==0)
                printf("You ordered a standard room.\n\n");
            else
                printf("You ordered a luxury room.\n\n");
            printf("\nRoom Info：\n");
            printf("The room number is ：%d\n\n",p->no);
        }
        p->next=*p_rlist_h;
        *p_rlist_h=p;
    }
    else // if the number of room is no less than the number limited, there is no space to build new room
    {
        printf("\nNo space for building roomn.\n\n");
    }
}

void removeRoom(Room **p_rlist_h, int *p_room_no)
{
    Room *ptr, *pre;
    char input[100];
    int i;
    
    if ((*p_room_no)==0)
    {
        printf("\nSorry, there is no room now.\n\n");
        return;
    }
    while (1)
    {
        printf("\nPlease enter the room number you want to remove:\n");
        gets(input);
        i = checkNumber(input);
        if (i<0)
            return;
        
        ptr = *p_rlist_h;
        pre = 0;
        while (ptr!= 0)
        {
            if (ptr->no == i)
                break;
            pre = ptr;
            ptr = ptr->next;
        }
        
        
        if (ptr==0) {
            invalidInput();
            return;
        }
        if (ptr->p_customer != 0) // if there is the room is occupied by a customer, it cannot be removed
        {
            printf("\nSorry, this room is not empty.\n\n");
            return;
        }
        break;
    }
    
    if (pre == 0)
    {
        *p_rlist_h = ptr->next;
        free(ptr);
    }
    else
    {
        pre->next = ptr->next;
        free(ptr);
    }
    (*p_room_no) -= 1;
    
    printf("\nThe room %d has been removed.\n\n",i);
}

void findRoom(Customer *clist_h) // find the room number given the customer's name
{
    char input[100];
    printf("\nPlease enter the customer's name:\n");
    gets(input);
    Customer *p;
    p = clist_h;
    while (p!=0)
    {
        if (strcmp(p->name,input)==0)
        {
            printf("\nThe room number of Customer %s is %d.\n\n", p->name, p->p_room->no);
            return;
        }
        p = p->next;
    }
    printf("\nSorry, we can't find the name in customer list.\n\n");
}

void findOccupier(Room *rlist_h) // find the name of the customer given the room number
{
    char input[100];
    int i;
    Room *r;

    while (1)
    {
        printf("\nPlease enter the room number:\n");
        gets(input);
        i = checkNumber(input);
        if (i<0)
        {
            continue;
        }
        
        r = rlist_h;
        while (r!=0)
        {
            if (r->no == i)
                break;
            r = r->next;
        }
        
        if (r==0)
        {
            invalidInput();
            return;
        }
        if (r->p_customer == 0)
            printf("\nThe room is empty.\n\n");
        else
            printf("\nThe occupier of Room %d is %s.\n\n", r->no, r->p_customer->name);
        break;
    }

}