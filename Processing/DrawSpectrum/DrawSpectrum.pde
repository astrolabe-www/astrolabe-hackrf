import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

Table table;
ArrayList<PVector> mRanges;
ArrayList<PVector> mLimits;
ArrayList<PowerPoint> freqsVals;
ArrayList<PowerPoint> freqsAverageVals;

float [] averageVals;
float currentAverageSum;
int currentAverageIndex;

String filename = "out-tgh-ear";

int currentAverageSize;
int AVERAGE_SIZE_MAX = 150;
int AVERAGE_SIZE_SAVE = 128;

void setup() {
  size(1000, 1000);
  background(255);
  stroke(0, 200);
  smooth(8);

  currentAverageSize = 2;

  table = loadTable(filename + ".csv");
  mRanges = new ArrayList<PVector>();
  mLimits = new ArrayList<PVector>();
  freqsVals = new ArrayList<PowerPoint>();
  freqsAverageVals = new ArrayList<PowerPoint>();

  mRanges.add(new PVector(50, 750));
  mRanges.add(new PVector(750, 1000));
  mRanges.add(new PVector(1000, 2000));
  mRanges.add(new PVector(2000, 3000));

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

  averagePoints(freqsVals, freqsAverageVals, AVERAGE_SIZE_SAVE);
  drawPoints(freqsAverageVals);
  save(dataPath(filename + "-" + AVERAGE_SIZE_SAVE + ".jpg"));
}

void averagePoints(ArrayList<PowerPoint> rawPoints, ArrayList<PowerPoint> averagePoints, int average_size) {
  if (average_size < 1) return;

  averagePoints.clear();
  mLimits.clear();

  averageVals = new float [average_size];
  for (int i = 0; i < averageVals.length; i++) {
    averageVals[i] = 0;
  }
  currentAverageSum = 0;
  currentAverageIndex = 0;

  for (int i = 0; i < mRanges.size(); i++) {
    mLimits.add(new PVector(1e6, -1e6));
  }

  // average vals and record max/min for each range
  for (int i = 0; i < rawPoints.size(); i++) {
    float rawP = rawPoints.get(i).power;

    currentAverageSum -= averageVals[currentAverageIndex];
    averageVals[currentAverageIndex] = rawP;
    currentAverageSum += averageVals[currentAverageIndex];
    currentAverageIndex = (currentAverageIndex + 1) % average_size;

    if (i >= (average_size / 2)) {
      PowerPoint mPP = rawPoints.get(i - (average_size / 2));
      float f = mPP.frequency;
      float p = currentAverageSum / average_size;
      int radiusGroup = mPP.radiusGroup;

      averagePoints.add(new PowerPoint(f, p, radiusGroup));

      if (p < mLimits.get(radiusGroup).x) mLimits.get(radiusGroup).x = p;
      if (p > mLimits.get(radiusGroup).y) mLimits.get(radiusGroup).y = p;
    }
  }
}

void drawPoints(ArrayList<PowerPoint> averagePoints) {
  float subRadius = 0.5 * width / mRanges.size();
  pushMatrix();
  translate(width/2, height/2);
  stroke(70);

  Collections.reverse(averagePoints);
  for (int i = 0; i < averagePoints.size(); i++) {
    pushMatrix();
    PowerPoint mVals = averagePoints.get(i);
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
  Collections.reverse(averagePoints);
  popMatrix();
}


void draw() {
  background(255);
  averagePoints(freqsVals, freqsAverageVals, abs(currentAverageSize));
  drawPoints(freqsAverageVals);

  currentAverageSize = currentAverageSize + 2;
  if (currentAverageSize > AVERAGE_SIZE_MAX) {
    currentAverageSize = -AVERAGE_SIZE_MAX;
  }
  delay(1);
}
