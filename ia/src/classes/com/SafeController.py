# coding = utf-8

from src.classes.com.Controller import Controller
from src.classes.com.Transaction import Transaction


class SafeController(object):

    @staticmethod
    def execute(transaction):
        if not issubclass(type(transaction), Transaction):
            raise TypeError("Not a transaction")
        transaction.execute()


safe_controller = SafeController()
