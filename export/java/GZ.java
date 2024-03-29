/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.1.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.seantone.sxtwl;

public class GZ {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected GZ(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(GZ obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(GZ obj) {
    long ptr = 0;
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new RuntimeException("Cannot release ownership as memory is not owned");
      ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.delete();
    }
    return ptr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        sxtwlJNI.delete_GZ(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public GZ() {
    this(sxtwlJNI.new_GZ__SWIG_0(), true);
  }

  public GZ(short tg, short dz) {
    this(sxtwlJNI.new_GZ__SWIG_1(tg, dz), true);
  }

  public void setTg(short value) {
    sxtwlJNI.GZ_tg_set(swigCPtr, this, value);
  }

  public short getTg() {
    return sxtwlJNI.GZ_tg_get(swigCPtr, this);
  }

  public void setDz(short value) {
    sxtwlJNI.GZ_dz_set(swigCPtr, this, value);
  }

  public short getDz() {
    return sxtwlJNI.GZ_dz_get(swigCPtr, this);
  }

}
