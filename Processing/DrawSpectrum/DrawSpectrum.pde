import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

Table table;
ArrayList<PVector> mRanges;
ArrayList<PVector> mLimits;
ArrayList<PVector> freqsVals;

String filename = "out-tgh-ear-avg";

Comparator<PVector> compareByX = new Comparator<PVector>() {
  @Override
    public int compare(PVector p0, PVector p1) {
    return int(p0.x - p1.x);
  }
};

void setup() {
  size(1000, 1000);
  background(255);
  stroke(0, 200);
  smooth(4);

  table = loadTable(filename + ".csv");
  mRanges = new ArrayList<PVector>();
  mLimits = new ArrayList<PVector>();
  freqsVals = new ArrayList<PVector>();

  mRanges.add(new PVector(50, 750));
  mRanges.add(new PVector(750, 1000));
  mRanges.add(new PVector(1000, 2000));
  mRanges.add(new PVector(2000, 2700));

  for (int i=0; i < mRanges.size(); i++) {
    mLimits.add(new PVector(1e6, -1e6));
  }

  for (TableRow row : table.rows()) {
    float f = row.getFloat(0);
    float p;
    try {
      p = row.getFloat(2);
    }
    catch(Exception e) {
      p = row.getFloat(1);
    }

    int r = 0;
    for (int i = 0; i < mRanges.size(); i++) {
      if (f < mRanges.get(i).y) {
        r = i;
        break;
      }
    }

    if (p < mLimits.get(r).x) mLimits.get(r).x = p;
    if (p > mLimits.get(r).y) mLimits.get(r).y = p;

    freqsVals.add(new PVector(f, p));
  }

  Collections.sort(freqsVals, compareByX);

  float subRadius = 0.5 * width / mRanges.size();

  pushMatrix();
  translate(width/2, height/2);

  Iterator<PVector> freqIter = freqsVals.iterator();

  while (freqIter.hasNext()) {
    PVector mVals = freqIter.next();
    pushMatrix();
    float f = mVals.x;
    float p = mVals.y;

    int r = 0;
    for (int i = 0; i < mRanges.size(); i++) {
      if (f < mRanges.get(i).y) {
        r = i;
        break;
      }
    }

    float angle = map(f, mRanges.get(r).x, mRanges.get(r).y, 0, TWO_PI); 
    float lineLength = map(p, mLimits.get(r).x, mLimits.get(r).y, 1, subRadius);
    float lineStart = r * subRadius;

    rotate(angle);
    line(lineStart, 0, lineStart + lineLength, 0);
    popMatrix();
  }

  popMatrix();
  save(dataPath(filename+".jpg"));
}

void draw() {
  delay(500);
  exit();
}
