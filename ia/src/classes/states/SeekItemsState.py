from ia.src.classes.states.StateMachine import AAIState, statemachine
from ia.src.classes.com.Controller import controller, Resources
from ia.src.classes.ia_res.Ant import ant


class SeekItemsState(AAIState):

    def goToIdx(self, idx):
        self.path = []
        pass

    def updateAntLook(self, look):
        print(look)

    def updateAntInventory(self, inventory):
        ant.inventory = inventory
        if self.food_floor > 0 and inventory[Resources.Food] < self.food_floor:
            def closureFood():
                statemachine.push(SeekItemsState({Resources.Food: self.food_floor}))

            statemachine.closure = closureFood

    def checkEnd(self):
        check = True

        def closurePop():
            statemachine.pop()

        for k, v in self.items_dict.items():
            if k not in ant.inventory or ant.inventory[k] < v:
                check = False
        if check:
            statemachine.closure = closurePop

    def __init__(self, items_dict, food_floor=-1):
        super().__init__("SeekItems")
        self.looked = False
        self.surface = max(ant.map_size.x, ant.map_size.y)
        self.items_dict = items_dict
        self.food_floor = food_floor
        self.path = []
