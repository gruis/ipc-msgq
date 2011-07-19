require File.join(File.dirname(__FILE__), "spec_helper")

describe Msgq do
  it "should have a VERSION" do
    Msgq.const_defined?(:VERSION).should be true
  end # it "should have a VERSION"
  describe "VERSION" do
    subject { Msgq::VERSION }
    it { should be_a String }
  end # describe "VERSION"
end # describe Msgq
