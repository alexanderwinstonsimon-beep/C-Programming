#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    char id[20];
    char name[50];
    char user[30];
    char pass[30];
    char status[20];
    char contact[30];
} Student;

typedef union {
    float amount;
    char  voucher[20];
} PayType;

typedef struct RateNode {
    char stuId[20];
    int  classId;
    int  score;
    char comment[100];
    struct RateNode *next;
} RateNode;

void showStudent(Student *s);
RateNode *addRating(RateNode *head, char *stuId, int classId, int score, char *comment);
void showRatingList(RateNode *head);
void clearRatings(RateNode *head);

int login();
int bookClass();
int payment();
int rating();
int showBookedClasses();

int loadStudents();
int loadClasses();
int saveClasses();
int loadPayments();
int savePayments();
int loadRatings();
int saveRatings();

int studentMenu();

#endif