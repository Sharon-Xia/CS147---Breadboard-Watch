from stepCounter import stepCounter
from bpmTracker import bpmTracker

class watchData:
    

    def __init__(self, user = 'default'):
        self.stepCounter = stepCounter(user)
        self.bpmTracker = bpmTracker(user)
        self.user = user
    

    def getUser(self):
        return self.user

    def getStepCounter(self):
        return self.stepCounter

    def getBpmTracker(self):
        return self.bpmTracker

    def updateData(self, steps, bpm):
        self.stepCounter.addSteps(steps)

        if bpm != -1:
            self.bpmTracker.addBpm(bpm)