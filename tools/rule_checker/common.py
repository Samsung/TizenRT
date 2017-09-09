class Result:
    def __init__(self, exitcode=0, message='', data={}):
        self.exitcode = exitcode
        self.message = message
        self.data = data
