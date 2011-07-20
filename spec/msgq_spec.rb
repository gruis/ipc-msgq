require File.join(File.dirname(__FILE__), "spec_helper")

describe Ipc::MsgQ do
  it "should have a VERSION" do
    Ipc::MsgQ.const_defined?(:VERSION).should be true
  end # it "should have a VERSION"
  describe "VERSION" do
    subject { Ipc::MsgQ::VERSION }
    it { should be_a String }
  end # describe "VERSION"

  it { Ipc::MsgQ.should respond_to(:ftok) }

  describe ".ftok" do
    it "should return a Fixnum on success" do
      Ipc::MsgQ.ftok(File.expand_path(__FILE__), 1).should be_a Fixnum
    end # it should return a Fixnum on success  

    it "should raise an argument error when the path doesn't exist" do
      expect {
        Ipc::MsgQ.ftok(File.expand_path(__FILE__) + ".missing", 1).should be_a Fixnum
      }.to raise_error(ArgumentError)
    end # it should raise an argument error when the path doesn't exist

    it "should not require an id" do
      expect {
         Ipc::MsgQ.ftok(File.expand_path(__FILE__))
      }.to_not raise_error(ArgumentError)
    end # it should not require an id
    
    it "should return the same key for multiple calls" do
      Ipc::MsgQ.ftok(File.expand_path(__FILE__)).should == Ipc::MsgQ.ftok(File.expand_path(__FILE__))
    end # it should return the same key for multiple calls
    it "should return different keys when the id is different" do
      Ipc::MsgQ.ftok(File.expand_path(__FILE__),7).should_not == Ipc::MsgQ.ftok(File.expand_path(__FILE__))
    end # it should return different keys when the id is different  
  end # ".ftok"

  describe ".msgget" do
    it "should return a q id"
    it "should accpet IPC_CREAT and IPC_EXCL"  
  end # ".msgget"
  
  describe ".status" do
    it "should return a hash"
    it "should have :count, :len, :lspid, :lrpid, :lstime, :rtime and :ctime"
  end # ".status"
end # describe Ipc::MsgQ
