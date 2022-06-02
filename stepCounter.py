
from datetime import date, datetime
from fileStorage import writeDataToFile


def getCurrentDay():
    now = datetime.now()
    now = now.replace(second=0, microsecond=0)
    return now

class stepCounter:

    def __init__(self, user, height = 5.5, age = 20, weight = 130):
        self.recordOfSteps = {}
        self.lifetimeSteps = 0

        self.steps = 0
        self.currentDay = getCurrentDay()
        self.user = user
        self.height = height
        self.age = age
        self.weight = weight

        self.recordOfSteps[self.currentDay] = 0

    
    
    def addSteps(self, numSteps):
        if self.currentDay != getCurrentDay():
            # record prev days to be displayed
            writeDataToFile(self.user, 'steps', self.currentDay, self.recordOfSteps[self.currentDay])

            self.currentDay = getCurrentDay()
            self.recordOfSteps[self.currentDay] = 0
            
        self.recordOfSteps[self.currentDay] += numSteps
        self.lifetimeSteps += numSteps
        return self.recordOfSteps[self.currentDay]

    def getSteps(self, date):
        if date in self.recordOfSteps.keys():
            return self.recordOfSteps[date]
        return -1

    def getCurrentDay(self):
        return self.currentDay
    



    