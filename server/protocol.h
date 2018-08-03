//protocol descripstion
typedef struct
{
    int id;
}Header;

//personal info
#define Msg_PersonInfoReq 1
typedef struct
{
    Header  header;
    int age;
    int atk;
}PersonInfoReq;

typedef struct
{
    Header  header;
    int age;
    int atk;
}PersonInfoAck;
