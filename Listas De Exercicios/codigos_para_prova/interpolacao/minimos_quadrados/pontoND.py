class PontoND:
    def __init__(self, coordinates: list[float]):
        self.coordinates = coordinates
        
    def print(self) -> None:
        print(f'({", ".join(map(str, self.coordinates))})')
        
    def __str__(self):
        return f'({", ".join(map(str, self.coordinates))})'
    
    def get_dimension(self) -> int:
        return len(self.coordinates)
    
    def get_coordinate(self, index: int) -> float:
        return self.coordinates[index]
            
class ListaPontosND:
    def __init__(self, dimension: int = 3):
        self.pontos = []
        self.dimension = dimension
        
    @property
    def size(self) -> int:
        return len(self.pontos)
    
    @staticmethod
    def from_coordinates_list(coordinates_list: list[list[float]]) -> 'ListaPontosND':
        lista = ListaPontosND()
        for coordinates in coordinates_list:
            if len(coordinates) != lista.dimension:
                raise ValueError("Todas as coordenadas devem ter a mesma dimensão.")
            lista.push(PontoND(coordinates))
        return lista
    
    def push(self, ponto: PontoND) -> None:
        if ponto.get_dimension() != self.dimension:
            raise ValueError("A dimensão do ponto não corresponde à dimensão da lista.")
        self.pontos.append(ponto)
    
    def get_pontos(self) -> list[PontoND]:
        return self.pontos
    
    def print(self) -> None:
        for p in self.pontos:
            p.print()
            
    def get_index_coordinate_values(self, index: int) -> list[float]:
        return [p.get_coordinate(index) for p in self.pontos]