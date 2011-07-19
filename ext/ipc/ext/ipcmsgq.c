#include <ruby/ruby.h>
#include <sys/ipc.h>

static VALUE
rb_ipc_msgq_ftok(VALUE mq, VALUE path, VALUE id)
{
    char *p = RSTRING_PTR(path);
    key_t k = ftok(p, NUM2INT(id));
    if(k == -1)
        rb_raise(rb_eArgError,"path '%s' not found", p);
    return INT2FIX(k);
}
void
Init_msgq()
{
    VALUE rb_mIpc;
    VALUE rb_cIpcMsgQ;
    
    rb_mIpc = rb_define_module("Ipc");
    rb_cIpcMsgQ = rb_define_class_under(rb_mIpc, "MsgQ", rb_cObject);
    rb_define_const(rb_cIpcMsgQ, "IPC_CREAT", INT2NUM(IPC_CREAT));
    rb_define_const(rb_cIpcMsgQ, "IPC_EXCL", INT2NUM(IPC_EXCL));
    rb_define_singleton_method(rb_cIpcMsgQ, "ftok", rb_ipc_msgq_ftok, 2);
}
