import numpy as np

class Mesh(object):
    '''
    Класс для обработки вокселей, для считывания из файлов датасета
    '''
    def __init__(self, vox, axis, translate, scale, axis_order):
        self.voxels = vox
        self.axis = axis
        self.translate = translate
        self.scale = scale 
        self.axis_order = axis_order

    
def read_header(FILE):
    '''
    Input: FILE
    Return: (axis, translate, scale) - поля для создания вокселя
    Принимает файл типа .binvox, считывает заголовок, данные для создания вокселей
    '''
    _ = FILE.readline()
    axis = [int(dim) for dim in FILE.readline().strip().split(b' ')[1:]]
    translate = [float(trans) for trans in FILE.readline().strip().split(b' ')[1:]]
    scale = [float(scal) for scal in FILE.readline().strip().split(b' ')[1:]][0]
    _ = FILE.readline()
    return axis, translate, scale

def read_mesh(FILE):
    '''
    Input: FILE
    Retrun: voxel - готовый воксель
    Принимает файл типа .binvox, возвращает воксель в формате 3-D numpy array
    '''
    axis, translate, scale = read_header(FILE)
    raws = np.frombuffer(FILE.read(), dtype=np.uint8) 
    data, count = raws[::2], raws[1::2]
    data = np.repeat(data, count).astype(np.bool_).reshape(axis).transpose(0, 2, 1)
    axis_order = 'xyz'
    return Mesh(data, axis, translate, scale, axis_order)

def voxel_to_mesh(voxels, surface_view):
    '''
    Input: voxels - объект типа Mesh
           surface_view - bool
    Return: (verts, faces) - вершины и грани сетки, созданные на основе вокселей

    Прнимает воксели, преобразует в формат для записи в .obj файл
    '''
    verts_for_cube = np.array([[0, 0, 0], [0, 0, 1], [0, 1, 0], [0, 1, 1], [1, 0, 0], 
                    [1, 0, 1], [1, 1, 0], [1, 1, 1]])

    faces_for_cube = np.array([[0, 1, 2], [1, 3, 2], [2, 3, 6], [3, 7, 6], [0, 2, 6], [0, 6, 4], [0, 5, 1],
                  [0, 4, 5], [6, 7, 5], [6, 5, 4], [1, 7, 3], [1, 5, 7]]) + 1

    verts = []
    faces = []
    curr_vert = 0

    positions = np.where(voxels > 0.3)
    voxels[positions] = 1 
    for i, j, k in zip(*positions):
        # identifies if current voxel has an exposed face 
        if not surface_view or np.sum(voxels[i-1:i+2, j-1:j+2, k-1:k+2]) < 27:
            verts.extend(0.01 * (verts_for_cube + 1.1 * np.array([[i, j, k]])))
            faces.extend(faces_for_cube + curr_vert)
            curr_vert += len(verts_for_cube)  
              
    return np.array(verts), np.array(faces)

def voxel_to_obj(fname, voxels, surface_view=True):
    '''
    Input: fname - string
           voxels - объект типа Mesh
           surface_view - bool
    Создает файл типа .obj
    '''
    (verts, faces) = voxel_to_mesh(voxels, surface_view)
    F = open(fname, "w")
    F.write('g\n# {} vertex\n'.format(len(verts)))
    for vert in verts:
        F.write('v {} {} {}\n'.format(vert[0], vert[1], vert[2]))

    F.write('# {} faces\n'.format(len(faces)))
    for face in faces:
        F.write('f {} {} {}\n'.format(face[0], face[1], face[2]))

if __name__ == "__main__":
    FILE = open("test.binvox", "rb")
    voxels = read_mesh(FILE)
    voxel_to_obj("test.obj", voxels.voxels)