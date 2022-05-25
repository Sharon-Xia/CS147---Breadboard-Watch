
from datetime import date



class stepCounter:

    def __init__(self, height = 5.5, age = 20, weight = 130):
        self.recordOfSteps = {}
        self.lifetimeSteps = 0

        self.steps = 0
        self.currentDay = date.today()
        self.height = height
        self.age = age
        self.weight = weight

    
    def addSteps(self, numSteps):
        self.steps += numSteps
        return self.steps


    def newDay(self):
        if self.currentDay != date.today():
            self.recordOfSteps[self.currentDay] = self.steps 
            self.lifetimeSteps += self.steps
            self.steps = 0
            self.currentDay = date.today()

    



    