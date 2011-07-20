require "ipc/ext/msgq"
module Ipc
  class MsgQ
    
    def initialize(path_or_id)
      @key = path_or_id.is_a?(String) ? Ipc::MsgQ.ftok(path_or_id) : path_or_id
      @qid = MsgQ.msgget(@key, 0644 | IPC_CREAT)
    end

    # Retrieve all avaiable status information about the queue
    # @return [Hash] :count, :bytes, :perm => [:cuid, :cgid, :uid, :gid, :mode] :lspid, :lrpid, :stime, :rtime, :ctime
    def status
      MsgQ.status(@qid)
    end

    # Count the number of message on the queue
    # @return [Fixnum]
    def count
      MsgQ.count(@qid)
    end
    
    def delete
      MsgQ.delete(@qid)
    end

    # Send a message and block if the queue is not ready to receive the message.
    # @return [Fixnum] the number of bytes sent
    def snd(msg)
      MsgQ.msgsnd(@qid, msg)
    end

    # Send a message without blocking
    # @return [Fixnum] the number of bytes sent
    def snd_nowait(msg)
      MsgQ.msgsnd_nowait(@qid, msg)
    end
    
    def rcv
      MsgQ.msgrcv(@qid)
    end
    
    def rcv_nowait
      MsgQ.msgrcv_nowait(@qid)
    end
    
  end # class::MsgQ
end # module::Ipc