//protocol descripstion
typedef struct
{
    int id;
}Header;

//personal info
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
