#include <ruby/ruby.h>
#include <sys/ipc.h>

static VALUE
rb_ipc_msgq_ftok(int argc, VALUE* argv, VALUE self)
{
    VALUE v_path, v_id;
    if(rb_scan_args(argc, argv, "11", &v_path, &v_id) == 1)
        v_id = INT2NUM(33);
    char *path = RSTRING_PTR(v_path);
    key_t k    = ftok(path, NUM2INT(v_id));
    if(k == -1)
        rb_raise(rb_eArgError,"path '%s' not found", path);
    return INT2NUM(k);
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
    rb_define_singleton_method(rb_cIpcMsgQ, "ftok", rb_ipc_msgq_ftok, -1);
}
