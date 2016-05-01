class Cell(object):
    def __init__(self, x, y):
        self.x, self.y = x, y
        self.is_mine = False
        self.is_clicked = False
        self.is_flagged = False
        self.num_surrounding = 0

class Board(object):
    def get_x_y(self, position):
        """Return x, y coordinate of a position in list."""
        return (position % self.width, position / self.width)

    def get_position(self, x, y):
        return (y* self.width) + x

    def get_cell(self, x, y):
        return self.board[self.get_position(x, y)]

    def neighbors(self, x, y):
        if x > 0:
            yield self.get_cell(x-1, y)
        if y > 0:
            yield self.get_cell(x, y-1)
        if x < self.width - 1:
            yield self.get_cell(x+1, y)
        if y < self.height -1:
            yield self.get_cell(x, y+1)

    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.board = [Cell(*self.get_x_y(pos)) for pos in xrange(width * height)]

    def _fill_mines(self, num_mines):
        mine_cells = random.sample(self.board, num_mines)
        for each_cell in mine_cells:
            each_cell.is_mine = True
            for each_neighbor in self.neighbors(each_cell.x, each_cell.y):
                each_neighbor.num_surrouding += 1

    @classmethod
    def build_board(cls, width, height, num_mines):
        board = cls(width, height)
        board._fill_mines(num_mines)

