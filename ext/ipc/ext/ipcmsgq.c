#include <ruby/ruby.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
/**
 * @example
 *  Ipc::MsgQ.msgctl(Ipc::MsgQ.msgget(Ipc::MsgQ.ftok("/tmp/actors/caleb.stuff"), Ipc::MsgQ::IPC_CREAT), Ipc::MsgQ::IPC_RMID)
 * @example
 *  Ipc::MsgQ.msgctl(Ipc::MsgQ.msgget(0x210202a1, Ipc::MsgQ::IPC_CREAT), Ipc::MsgQ::IPC_RMID)
 */
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

static VALUE
rb_ipc_msgq_msgget(VALUE self, VALUE v_kid, VALUE v_flags)
{
    int q;
    if( (q = msgget(NUM2INT(v_kid), NUM2INT(v_flags))) == -1 )
        rb_sys_fail("message queue already exists");
    return INT2NUM( q );
}

static VALUE
rb_ipc_msgq_msgctl(int argc, VALUE* argv, VALUE self)
{
    VALUE v_id, v_cmd, v_stat;
    rb_scan_args(argc, argv, "21", &v_id, &v_cmd, &v_stat); /* @todo set v_stat if necessary */
    struct msqid_ds msq_status;

    if(msgctl((key_t)NUM2INT(v_id), NUM2INT(v_cmd), &msq_stat) == -1)
        rb_sys_fail("msgctl failed");

    return Qnil;
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
    rb_define_const(rb_cIpcMsgQ, "IPC_STAT", INT2NUM(IPC_STAT));
    rb_define_const(rb_cIpcMsgQ, "IPC_SET", INT2NUM(IPC_SET));
    rb_define_const(rb_cIpcMsgQ, "IPC_RMID", INT2NUM(IPC_RMID));
    rb_define_singleton_method(rb_cIpcMsgQ, "ftok", rb_ipc_msgq_ftok, -1);
    rb_define_singleton_method(rb_cIpcMsgQ, "msgget", rb_ipc_msgq_msgget, 2);
    rb_define_singleton_method(rb_cIpcMsgQ, "msgctl", rb_ipc_msgq_msgctl, -1);
}
