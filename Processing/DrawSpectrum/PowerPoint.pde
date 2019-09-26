
public final class PowerPoint implements Comparable<PowerPoint> {
  private PVector mPoint;
  public float frequency;
  public float power;
  public int radiusGroup;

  public PowerPoint(float _f, float _p, float _rg) {
    this.mPoint = new PVector(_f, _p, _rg);
    frequency = _f;
    power = _p;
    radiusGroup = int(_rg);
  }

  @Override
  public int compareTo(PowerPoint pp1) {
    return int(this.frequency - pp1.frequency);
  }

  public float frequency() {
    return this.mPoint.x;
  }
  public float power() {
    return this.mPoint.y;
  }
  public int radiusGroup() {
    return int(this.mPoint.z);
  }
};
