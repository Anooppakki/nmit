#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hotel.h"


int main()
{
    char hotel_name[30] = "Andhra Vindu"; 
    char hotel_address[100] = "Yelahanka"; 
    int room_no = 0; 
    Room *rlist_h = 0; 
    Customer *clist_h = 0; 
    printf("Welcome to %s!\n\n",hotel_name);
  char input[100]; 
    while (1)
    {
        printf("\nWhat sevice do you need?\n(Please enter the number of the sevice)\n");
        printf("1. Check-in\n");
        printf("2. Check-out\n");
        printf("0. Exit the program\n");
        gets(input);
        int i;
        i = checkNumber(input); 
        if (i<0)
            continue;
		Customer *p;
        switch (i) 
        {
            case 1:
                check_in(&clist_h,rlist_h);
                break;
            case 2:
                check_out(&clist_h);
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

int checkNumber(char s[100]) 
{
    int flag,ans;
    flag = sscanf(s,"%d",&ans);
   if ( (flag<=0) || (ans<0)/*|| (strlen(s)!=1) */)
    {
        invalidInput();
        return -1;
    }
    return ans;

}

void invalidInput() 
{
    printf("\nSorry, your input is invalid.\nPlease enter the correct instruction again.\n\n");
}

void exit_program()
{
    printf("\nThank you for using this program!\n");
}

Customer *findCustomer(Customer *clist_h)
{
    char input[100];
    printf("Please enter your name:\n");
    gets(input);
    Customer *p;
    p = clist_h;
    while (p!=0) 
    {
        if (strcmp(p->name,input)==0) {
            return p;
        }
        p = p->next;
    }
    printf("\nSorry, we can't find your name in customer list. Please check-in first.\n\n");
    return 0;
    
}

void check_in(Customer **p_clist_h, Room *rlist_h)
{
    int sremain = 1, lremain = 1;
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
    
    if ((sremain==0)&&(lremain==0)) 
    {
        printf("\nSorry, there is no room available now.\n\n");
        return;
    }
    printf("\nThere are %d standard rooms available now.\n",sremain);
    printf("There are %d luxury rooms available now.\n",lremain);
    
    int room_type = -1;
    while (1)
    {
        printf("Which type of room do you prefer?\n");
        if (sremain>0)
            printf("1. Standard room (1700/day, 10%% room sevice fee）\n");
        if (lremain>0)
            printf("2. Luxury room   (4000/day, 10%% room sevice fee）\n");
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
    
    Customer *c; 
    char input[100];
    c = (Customer *)malloc(sizeof(Customer));
    printf("Please enter your name:\n");
    gets(input);
    strcpy(c->name,input);
    printf("Please enter your address:\n");
    gets(input);
    strcpy(c->address, input);
    printf("Please enter your ID/passport number:\n"); 
    gets(input);
    strcpy(c->id_no, input);
    while (1)
    {
        printf("Please enter the date today: (1-30)\nDay ");
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

void check_out(Customer **p_clist_h) 
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
        printf("Please enter the date today: (2-31)\nDay "); 
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
    printf("\nThis is your receipt:\n"); 
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
    printf("Subtotal: %34.s%10.1lf\n","HK$",subtotal);
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
