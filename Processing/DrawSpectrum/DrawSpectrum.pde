import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

Table table;
ArrayList<PVector> mRanges;
ArrayList<PVector> mLimits;
ArrayList<PowerPoint> freqsVals;
ArrayList<PowerPoint> freqsAverageVals;

int AVERAGE_SIZE = 128;
float [] averageVals;
float currentAverageSum;
int currentAverageIndex;

String filename = "out-tgh-ear";

void setup() {
  size(1000, 1000);
  background(255);
  stroke(0, 200);
  smooth(8);

  table = loadTable(filename + ".csv");
  mRanges = new ArrayList<PVector>();
  mLimits = new ArrayList<PVector>();
  freqsVals = new ArrayList<PowerPoint>();
  freqsAverageVals = new ArrayList<PowerPoint>();

  averageVals = new float [AVERAGE_SIZE];
  for (int i = 0; i < averageVals.length; i++) {
    averageVals[i] = 0;
  }

  currentAverageSum = 0;
  currentAverageIndex = 0;

  mRanges.add(new PVector(50, 750));
  mRanges.add(new PVector(750, 1000));
  mRanges.add(new PVector(1000, 2000));
  mRanges.add(new PVector(2000, 3000));

  float subRadius = 0.5 * width / mRanges.size();

  for (int i = 0; i < mRanges.size(); i++) {
    mLimits.add(new PVector(1e6, -1e6));
  }

  // parse and sort
  for (TableRow row : table.rows()) {
    float f = row.getFloat(0);
    float p = row.getFloat(1);

    int radiusGroup = 0;
    for (int ri = 0; ri < mRanges.size(); ri++) {
      if (f < mRanges.get(ri).y) {
        radiusGroup = ri;
        break;
      }
    }
    freqsVals.add(new PowerPoint(f, p, radiusGroup));
  }
  Collections.sort(freqsVals);
  mRanges.get(mRanges.size() - 1).y = freqsVals.get(freqsVals.size() - 1).frequency;

  // average vals and record max/min for each range
  for (int i = 0; i < freqsVals.size(); i++) {
    float rawP = freqsVals.get(i).power;

    currentAverageSum -= averageVals[currentAverageIndex];
    averageVals[currentAverageIndex] = rawP;
    currentAverageSum += averageVals[currentAverageIndex];
    currentAverageIndex = (currentAverageIndex + 1) % AVERAGE_SIZE;

    if (i >= (AVERAGE_SIZE / 2)) {
      PowerPoint mPP = freqsVals.get(i - (AVERAGE_SIZE / 2));
      float f = mPP.frequency;
      float p = currentAverageSum / AVERAGE_SIZE;
      int radiusGroup = mPP.radiusGroup;

      freqsAverageVals.add(new PowerPoint(f, p, radiusGroup));

      if (p < mLimits.get(radiusGroup).x) mLimits.get(radiusGroup).x = p;
      if (p > mLimits.get(radiusGroup).y) mLimits.get(radiusGroup).y = p;
    }
  }

  pushMatrix();
  translate(width/2, height/2);
  stroke(70);

  Collections.reverse(freqsAverageVals);
  for (int i = 0; i < freqsAverageVals.size(); i++) {
    pushMatrix();
    PowerPoint mVals = freqsAverageVals.get(i);
    float f = mVals.frequency;
    float p = mVals.power;
    int r = mVals.radiusGroup;

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
