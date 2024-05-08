/*
handler of inter process communication(a typical example is a process waiting for process to exit)
using a pipeline model (pretty similar to reading/writing to the terminal),
a read from an empty pipe will block the caller until another process write something to the pipe,
a read from a non-empty pipe will not block the caller
note that reading/writing can happen in any order (read->write or write->read)
 */

#include <list.h>
#include <debug.h>
#include "threads/synch.h"
#include "userprog/ipc.h"
#include "threads/malloc.h"

// pipe is identified by a pipe name and a ticket number.

struct ecriture_struct {
    int ticket;
    char *pipe_name;
    int msg; // return value to the reader, supporting only int for now since it's sufficient for the current use cases, and relieve from memory management
    struct list_elem elem;
};

struct lecture_struct {
    int ticket;
    char *pipe_name;
    struct semaphore sema; // the semaphore sync primitive is used to implement the blocking
    struct list_elem elem;
};

static struct list liste_ecriture;
static struct list liste_lecture;

void ipc_initialiser(void){
    list_init(&liste_lecture);
    list_init(&liste_ecriture);
}

int ipc_pipe_lecture(char * pipe_name, int ticket){
  // check whether there is already something written that correspond to the read 
  struct list_elem *e;
  for (e = list_begin (&liste_ecriture); e != list_end (&liste_ecriture); e = list_next (e))
  {
    struct ecriture_struct *w = list_entry (e, struct ecriture_struct, elem);
    if(w->pipe_name == pipe_name && ticket == w->ticket){
      list_remove(e);
      int msg = w->msg;
      free(w);
      return msg;
    }
  }
  // add an entry to the read list and block the caller
  struct lecture_struct *r = malloc(sizeof(struct lecture_struct));
  sema_init(&r->sema, 0);
  r->ticket = ticket;
  r->pipe_name = pipe_name;
  list_push_back(&liste_lecture, &r->elem);
  sema_down(&r->sema);
  // something written, re-parse the list again
  // TODO: possibility for code reuse
  for (e = list_begin (&liste_ecriture); e != list_end (&liste_ecriture); e = list_next (e))
  {
    struct ecriture_struct *w = list_entry (e, struct ecriture_struct, elem);
    if(w->pipe_name == r->pipe_name && r->ticket == w->ticket){
      list_remove(e);
      list_remove(&r->elem);
      int msg = w->msg;
      free(w);
      free(r);
      return msg;
    }
  }
  NOT_REACHED ();
}

void ipc_pipe_ecriture(char *pipe_name, int ticket, int msg){
  struct ecriture_struct *w= malloc(sizeof(struct ecriture_struct));
  w->ticket = ticket;
  w->pipe_name = pipe_name;
  w->msg = msg;
  list_push_back(&liste_ecriture, &w->elem);

  // check whether there is someone waiting for the write to up the semaphore
  struct list_elem *e;
  for (e = list_begin (&liste_lecture); e != list_end (&liste_lecture); e = list_next (e))
  {
    struct lecture_struct *r = list_entry (e, struct lecture_struct, elem);
    if(r->pipe_name == pipe_name && r->ticket == w->ticket){
      sema_up(&r->sema);
    }
  }
}
