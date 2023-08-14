import cn.ybits.pms.common.nfi.NativeProxy;

public class Another {

  public static void main(String[] args) {
    System.out.println(NativeProxy.welcomeMessage());
    System.out.println(NativeProxy.add(100, 100));
    System.out.println(NativeProxy.calcCost(1.06));
    System.out.println(NativeProxy.mailSecurityInfo());
  }
}
