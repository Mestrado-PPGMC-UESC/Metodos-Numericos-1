class Ponto2D:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y
        
    def print(self) -> None:
        print(f'({self.x}, {self.y})')
        
    def __str__(self):
        return f'({self.x}, {self.y})'
        
class ListaPontos2D:
    def __init__(self):
        self.pontos = []
        
    @property
    def size(self) -> int:
        return len(self.pontos)
    
    @staticmethod
    def from_x_y_values(x_values: list[float], y_values: list[float]) -> 'ListaPontos2D':
        if len(x_values) != len(y_values):
            raise ValueError("As listas de valores x e y devem ter o mesmo tamanho.")
        lista = ListaPontos2D()
        for x, y in zip(x_values, y_values):
            lista.push(Ponto2D(x, y))
        return lista
    
    def print(self) -> None:
        for p in self.pontos:
            p.print()
    
    def push(self, ponto: Ponto2D) -> None:
        self.pontos.append(ponto)
    
    def get_pontos(self) -> list[Ponto2D]:
        return self.pontos
    
    def get_x_values(self) -> list[float]:
        return [p.x for p in self.pontos]
    
    def get_y_values(self) -> list[float]:
        return [p.y for p in self.pontos]