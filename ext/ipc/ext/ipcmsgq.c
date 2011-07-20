#include <ruby/ruby.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>

/**
 * @example
 *  Ipc::MsgQ.msgctl(Ipc::MsgQ.msgget(Ipc::MsgQ.ftok("/tmp/actors/caleb.stuff"), 0644 | Ipc::MsgQ::IPC_CREAT), Ipc::MsgQ::IPC_RMID)
 * @example
 *  Ipc::MsgQ.msgctl(Ipc::MsgQ.msgget(0x210202a1, Ipc::MsgQ::IPC_CREAT), Ipc::MsgQ::IPC_RMID)
 # @example
 #  Ipc::MsgQ.bytes(q, 1024)
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
    struct msqid_ds msq_stat;

    if(msgctl((key_t)NUM2INT(v_id), NUM2INT(v_cmd), &msq_stat) == -1)
        rb_sys_fail("msgctl failed");

    return Qnil;
}

static VALUE
rb_ipc_msgq_status(VALUE self, VALUE v_qid)
{
    VALUE stat, perms;
    struct msqid_ds msq_stat;
    
    if( msgctl((key_t)NUM2INT(v_qid), IPC_STAT, &msq_stat) == -1 )
        rb_sys_fail("unable to retrieve status for MsgQ");

    perms = rb_hash_new();
    stat  = rb_hash_new();
    
    rb_hash_aset(perms, ID2SYM(rb_intern("cuid")), INT2NUM(msq_stat.msg_perm.cuid));
    rb_hash_aset(perms, ID2SYM(rb_intern("cgid")), INT2NUM(msq_stat.msg_perm.cgid));
    rb_hash_aset(perms, ID2SYM(rb_intern("uid")), INT2NUM(msq_stat.msg_perm.uid));
    rb_hash_aset(perms, ID2SYM(rb_intern("gid")), INT2NUM(msq_stat.msg_perm.gid));
    rb_hash_aset(perms, ID2SYM(rb_intern("mode")), INT2NUM(msq_stat.msg_perm.mode));
    
    rb_hash_aset(stat, ID2SYM(rb_intern("count")), INT2NUM(msq_stat.msg_qnum));
    rb_hash_aset(stat, ID2SYM(rb_intern("bytes")), INT2NUM(msq_stat.msg_qbytes));
    rb_hash_aset(stat, ID2SYM(rb_intern("perm")), perms);
    rb_hash_aset(stat, ID2SYM(rb_intern("lspid")), INT2NUM(msq_stat.msg_lspid));
    rb_hash_aset(stat, ID2SYM(rb_intern("lrpid")), INT2NUM(msq_stat.msg_lrpid));
    rb_hash_aset(stat, ID2SYM(rb_intern("stime")), INT2NUM(msq_stat.msg_stime));
    rb_hash_aset(stat, ID2SYM(rb_intern("rtime")), INT2NUM(msq_stat.msg_rtime));
    rb_hash_aset(stat, ID2SYM(rb_intern("ctime")), INT2NUM(msq_stat.msg_ctime));
    return stat;
}

static VALUE
rb_ipc_msgq_count(VALUE self, VALUE v_qid)
{
    struct msqid_ds msq_stat;
    if( msgctl((key_t)NUM2INT(v_qid), IPC_STAT, &msq_stat) == -1 )
        rb_sys_fail("unable to retrieve status for MsgQ");
    /* do i need to free the struct? */
    return INT2NUM(msq_stat.msg_qnum);
}

static VALUE
rb_ipc_msgq_set_mode(VALUE self, VALUE v_qid, VALUE mode)
{
    struct msqid_ds msq_stat;

    if( msgctl((key_t)NUM2INT(v_qid), IPC_STAT, &msq_stat) == -1 )
        rb_sys_fail("unable to retrieve status for MsgQ");

    msq_stat.msg_perm.mode = NUM2INT(mode);
    
    if( msgctl((key_t)NUM2INT(v_qid), IPC_SET, &msq_stat) == -1 )
        rb_sys_fail("unable to set mode");
    return Qnil;    
}

static VALUE
rb_ipc_msgq_set_bytes(VALUE self, VALUE v_qid, VALUE v_bytes)
{
    struct msqid_ds msq_stat;

    if( msgctl((key_t)NUM2INT(v_qid), IPC_STAT, &msq_stat) == -1 )
        rb_sys_fail("unable to retrieve status for MsgQ");

    msq_stat.msg_qbytes = NUM2INT(v_bytes);
    
    if( msgctl((key_t)NUM2INT(v_qid), IPC_SET, &msq_stat) == -1 )
        rb_sys_fail("unable to set bytes; are you root?");
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
    rb_define_singleton_method(rb_cIpcMsgQ, "status", rb_ipc_msgq_status, 1);
    rb_define_singleton_method(rb_cIpcMsgQ, "count", rb_ipc_msgq_count, 1);
    rb_define_singleton_method(rb_cIpcMsgQ, "mode", rb_ipc_msgq_set_mode, 2);
    rb_define_singleton_method(rb_cIpcMsgQ, "bytes", rb_ipc_msgq_set_bytes, 2);
}
