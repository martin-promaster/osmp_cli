package cn.ybits.pms.common.nfi;

public class NativeProxy {

    static
    {
        try {
            System.loadLibrary("pmsinvoker");
        }
        catch(UnsatisfiedLinkError e) {
            System.err.println(">>> Can not load library: " + e.toString());
        }
    }

    public static native int welcomeMessage();
    public static native int add(int a, int b);
    public static native double calcCost(double b);
    public static native String mailSecurityInfo();
}