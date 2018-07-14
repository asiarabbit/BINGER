#define BLOCK_LENGTH 8192
#define MAX_BLOCK       8

#define BLOCK_EMPTY     0
#define BLOCK_WRITING   1
#define BLOCK_WAITING   2
#define SHMKEY 76 

struct SHM_VME{
  int pid;
  unsigned int  runno;
  int  status;
  time_t  start_time;
  time_t  stop_time;
  unsigned int  block_no;
  unsigned int  event_no;
  char raw_filename[256];
  int raw_fd;
  struct {
    unsigned int status,blockno;
    unsigned int data[BLOCK_LENGTH];
  } buffer[MAX_BLOCK];
  int com_flag;
  time_t com_time;
  char com_filename[256];
  char com_comments[256];
  char com_message[256];
  int scalers[16];
  unsigned int event_length;
  unsigned int sca_length;
  int ana_status;  
  int ana_flag;
  char ana_message[256];
};


