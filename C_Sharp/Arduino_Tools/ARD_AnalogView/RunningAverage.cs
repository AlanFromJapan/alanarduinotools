using System;
using System.Collections.Generic;
using System.Text;

namespace ARD_AnalogView
{
    public class RunningAverage
    {
        private List<float> mValues = new List<float>();
        private int mSampleCount = 0;
        private int mSampleMax = 100;
        private float mRunningTotal = 0;

        public float Average {
            get
            {
                return this.mRunningTotal / (float)this.mSampleMax;
            }
        }

        public RunningAverage(int pSampleMax) {
            this.mSampleMax = pSampleMax;
        }

        public void NewSample(float pVal) {
            this.mRunningTotal += pVal;

            if (this.mValues.Count < this.mSampleMax)
            {
                //not full : add
                this.mValues.Add(pVal);
            }
            else { 
                //full : cycle replace

                //sub from running total
                this.mRunningTotal -= this.mValues[this.mSampleCount];

                //replace
                this.mValues[this.mSampleCount] = pVal;
            }

            this.mSampleCount = (this.mSampleCount + 1) % this.mSampleMax;
        }
    }
}
