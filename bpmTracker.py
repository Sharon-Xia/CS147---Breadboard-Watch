from datetime import date, datetime
from fileStorage import writeDataToFile

def getCurrentTime():
    now = datetime.now()
    now = now.replace(second=0, microsecond=0)
    return now

class bpmTracker:
    def __init__(self, user):

        self.user = user

        self.currentTime = getCurrentTime()
        self.bpmSum = 0
        self.n = 0 # num of bpms in sum

    # avg per minute
    def addBpm(self, bpm):
        if (self.currentTime != getCurrentTime() and self.n > 0):
            bpmAvg = int(self.bpmSum/self.n)
            writeDataToFile(self.user, 'bpm', self.currentTime, bpmAvg)
            self.bpmSum, self.n = 0, 0
            self.currentTime = getCurrentTime()

        self.bpmSum += bpm
        self.n += 1

        print(str(self.bpmSum) + ", " + str(self.n))

    def getCurrentBpm(self):
        if self.n > 0:
            return self.currentTime, int(self.bpmSum/self.n)

        return None, None
