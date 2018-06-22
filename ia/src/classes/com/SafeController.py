# coding = utf-8
from ia.src.classes.com.Controller import Resources
from ia.src.classes.ia_res.Ant import ant
from ia.src.classes.ia_res.TrackableTransactions import InventoryTransaction
from ia.src.classes.states.StateMachine import statemachine


class SafeController(object):

    def __init__(self):
        self.save = None
        self.endTransaction = None
        self.safe = True

    def clear_transaction(self, *args, **kwargs):
        endTransa = self.endTransaction
        self.save = None
        self.endTransaction = None
        endTransa(*args, **kwargs)

    def clear_state(self, cli):
        del cli
        self.safe = True
        self.safe_exec(self.save)

    def estimate_food(self, inventory):
        from ia.src.classes.states.SeekItemsState import SeekItemsState
        ant.inventory = inventory
        print("Food Level : ", inventory)
        if inventory[Resources.Food] < self.save.get_estimated_time() / 126 or inventory[Resources.Food] < 7:
            print("EMERGENCY MOD")
            self.safe = False
            state = SeekItemsState({Resources.Food: int(self.save.get_estimated_time() / 126 + 10)}, True)
            state.on_pop = self.clear_state
            statemachine.push(state)
        else:
            self.endTransaction = self.save.end
            self.save.end = self.clear_transaction
            self.save.execute()

    def safe_exec(self, transaction):
        self.save = transaction
        InventoryTransaction(self.estimate_food).execute()

    def execute(self, transaction):
        if self.save and self.safe:
            raise Exception("Invalid Concurrent transaction")
        if self.safe:
            self.safe_exec(transaction)
        else:
            transaction.execute()


safe_controller = SafeController()
