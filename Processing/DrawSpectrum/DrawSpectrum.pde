import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

Table table;
ArrayList<PVector> mRanges;
ArrayList<PVector> mLimits;
ArrayList<PVector> freqsVals;
ArrayList<PVector> freqsAverageVals;

int AVERAGE_SIZE = 128;
float [] averageVals;
float currentAverageSum;
int currentAverageIndex;

String filename = "out-tgh-ear";

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
  freqsAverageVals = new ArrayList<PVector>();

  averageVals = new float [AVERAGE_SIZE];
  for (int i = 0; i < averageVals.length; i++) {
    averageVals[i] = 0;
  }

  currentAverageSum = 0;
  currentAverageIndex = 0;

  mRanges.add(new PVector(50, 750));
  mRanges.add(new PVector(750, 1000));
  mRanges.add(new PVector(1000, 2000));
  mRanges.add(new PVector(2000, 2700));

  float subRadius = 0.5 * width / mRanges.size();

  for (int i = 0; i < mRanges.size(); i++) {
    mLimits.add(new PVector(1e6, -1e6));
  }

  // parse and sort
  for (TableRow row : table.rows()) {
    float f = row.getFloat(0);
    float p = row.getFloat(1);
    freqsVals.add(new PVector(f, p));
  }
  Collections.sort(freqsVals, compareByX);

  // average vals and record max/min for each range
  for (int i = 0; i < freqsVals.size(); i++) {
    PVector mVals = freqsVals.get(i);
    float rawP = mVals.y;

    currentAverageSum -= averageVals[currentAverageIndex];
    averageVals[currentAverageIndex] = rawP;
    currentAverageSum += averageVals[currentAverageIndex];
    currentAverageIndex = (currentAverageIndex + 1) % AVERAGE_SIZE;

    if (i >= (AVERAGE_SIZE / 2)) {
      float f = freqsVals.get(i - (AVERAGE_SIZE / 2)).x;
      float p = currentAverageSum / AVERAGE_SIZE;
      freqsAverageVals.add(new PVector(f, p));

      int r = 0;
      for (int ri = 0; ri < mRanges.size(); ri++) {
        if (f < mRanges.get(ri).y) {
          r = ri;
          break;
        }
      }

      if (p < mLimits.get(r).x) mLimits.get(r).x = p;
      if (p > mLimits.get(r).y) mLimits.get(r).y = p;
    }
  }


  pushMatrix();
  translate(width/2, height/2);

  for (int i = 0; i < freqsAverageVals.size(); i++) {
    pushMatrix();
    PVector mVals = freqsAverageVals.get(i);
    float f = mVals.x;
    float p = mVals.y;

    int r = 0;
    for (int ri = 0; ri < mRanges.size(); ri++) {
      if (f < mRanges.get(ri).y) {
        r = ri;
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
