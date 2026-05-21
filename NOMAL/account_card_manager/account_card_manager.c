// Name: account_card_manager.c
// Compile: gcc account_card_manager.c -o account_card_manager
// Build environment: Ubuntu 18.04
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 0x8

struct user{
    char name[0x20];
    char password[0x20];
    char memo[0xc0];
};

struct card{
    char title[0x40];
    char *msg;
    void (*printer)(char *) ;
    char desc[0xb0];
};

struct user *users[MAX];
struct card *cards[MAX];

int user_used[MAX];
int card_used[MAX];

void init(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int read_int(){
    char buf[0x20];

    memset(buf, 0, sizeof(buf));
    read(0, buf, sizeof(buf) - 1);

    return atoi(buf);
}

void read_data(char *buf, int size){
    int n = read(0, buf, size);

    if(n <= 0){
        exit(1);
    }
}

void card_print(char *msg){
    printf("[CARD] ");
    puts(msg);
}

void preview(){
    char buf[0x28];

    memset(buf, 'A', sizeof(buf));

    printf("Preview name> ");
    read_data(buf, 0x28);

    printf("Preview: ");
    write(1, buf, 0x30);
    printf("\n");
}

void create_user(){
    int idx;

    printf("User idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(user_used[idx]){
        puts("Already used");
        return;
    }

    users[idx] = (struct user *)malloc(sizeof(struct user));
    if(!users[idx]){
        exit(1);
    }

    memset(users[idx], 0, sizeof(struct user));
    user_used[idx] = 1;

    printf("Name> ");
    read_data(users[idx]->name, 0x20);

    printf("Password> ");
    read_data(users[idx]->password, 0x20);

    strcpy(users[idx]->memo, "empty memo");

    puts("User created");
}

void edit_user(){
    int idx;

    printf("User idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(!user_used[idx]){
        puts("No user");
        return;
    }

    printf("Memo> ");
    read_data(users[idx]->memo, 0xc0);

    puts("Memo updated");
}

void create_card(){
    int idx;

    printf("Card idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(card_used[idx]){
        puts("Already used");
        return;
    }

    cards[idx] = (struct card *)malloc(sizeof(struct card));
    if(!cards[idx]){
        exit(1);
    }

    memset(cards[idx], 0, sizeof(struct card));
    card_used[idx] = 1;

    printf("Title> ");
    read_data(cards[idx]->title, 0x40);

    printf("Description> ");
    read_data(cards[idx]->desc, 0xb0);

    cards[idx]->msg = cards[idx]->desc;
    cards[idx]->printer = card_print;

    puts("Card created");
}

void rename_card(){
    int idx;

    printf("Card idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(cards[idx] == NULL){
        puts("No card");
        return;
    }

    printf("New title> ");
    read_data(cards[idx]->title, 0x40);

    puts("Renamed");
}

void recover_card(){
    int idx;

    printf("Card idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(cards[idx] == NULL){
        puts("No backup");
        return;
    }

    if(card_used[idx]){
        puts("This card is not deleted.");
        return;
    }

    printf("Recovered title: ");
    write(1, cards[idx]->title, 0x50);
    printf("\n");
}

void delete_card(){
    int idx;

    printf("Card idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(cards[idx] == NULL){
        puts("No card");
        return;
    }

    free(cards[idx]);
    card_used[idx] = 0;

    puts("Card deleted");
}

int run_count = 0;

void run_card(){
    int idx;

    printf("Card idx> ");
    idx = read_int();

    if(idx < 0 || idx >= MAX){
        puts("Invalid idx");
        return;
    }

    if(!card_used[idx]){
        puts("No active card");
        return;
    }

    if(run_count){
        puts("Run limit exceeded");
        return;
    }

    run_count = 1;
    cards[idx]->printer(cards[idx]->msg);
}

void feedback(){
    char buf[0x80];

    printf("Feedback> ");
    read_data(buf, 0x200);

    puts("Thanks");
}

void menu(){
    puts("1. Preview");
    puts("2. Create user");
    puts("3. Edit user memo");
    puts("4. Create card");
    puts("5. Rename card");
    puts("6. Recover card");
    puts("7. Delete card");
    puts("8. Run card");
    puts("9. Feedback");
    puts("99. Exit");
    printf("> ");
}

int main(){
    init();

    while(1){
        menu();

        int sel = read_int();

        switch(sel){
        case 1:
            preview();
            break;
        case 2:
            create_user();
            break;
        case 3:
            edit_user();
            break;
        case 4:
            create_card();
            break;
        case 5:
            rename_card();
            break;
        case 6:
            recover_card();
            break;
        case 7:
            delete_card();
            break;
        case 8:
            run_card();
            break;
        case 9:
            feedback();
            break;
        case 99:
            puts("Bye");
            exit(0);
        default:
            puts("Invalid");
            break;
        }
    }

    return 0;
}
