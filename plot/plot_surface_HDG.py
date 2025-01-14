import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def read_points_from_file(file_path):
    points = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        num_points = int(lines[0].strip()) 
        for line in lines[1:num_points + 1]:  
            x, y = map(float, line.strip().split('\t'))  
            points.append((x, y))  
    return points

def read_traingles_from_file(file_path):
    traingles = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        num_triangles = int(lines[0].strip())
        for line in lines[1:num_triangles + 1]:
            t1, t2, t3 = map(float, line.strip().split('\t'))
            traingles.append((t1, t2, t3))
    return traingles

def read_solution_from_file(file_path):
    solution = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        for line in lines:
            s = float(line)
            solution.append([s])
    
    return solution

# path to source data
path_nodes        = "../build/mesh_nodes.txt"
path_edges        = "../build/mesh_edges.txt"
path_triangles    = "../build/mesh_triangles.txt"
path_solution     = "../build/calculated_solution_HDG.txt"
path_cor_solution = "../build/correct_solution_HDG.txt"


points        = np.array(read_points_from_file(path_nodes))
edges         = np.array(read_points_from_file(path_edges))
triangles     = np.array(read_traingles_from_file(path_triangles))
solution      = np.array(read_solution_from_file(path_solution))
corr_solution = np.array(read_solution_from_file(path_cor_solution))

edge_points = []
for edge in edges:
    edge_p1 = points[int(edge[0])]
    edge_p2 = points[int(edge[1])]
    edge_points.append((edge_p1 + edge_p2) / 2)
    
vertices = np.hstack([edge_points, solution[points.size : -1]])

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot_trisurf(vertices[:,0], vertices[:,1], vertices[:,2], triangles = triangles, cmap=plt.cm.cool)
ax.view_init(elev = 10, azim = 25)


plt.show()
plt.savefig("CalcSolution.png")

vectices_cor = np.hstack([points, corr_solution])
fig2 = plt.figure()
ax_cor = fig2.gca(projection = '3d')
ax_cor.plot_trisurf(vectices_cor[:,0], vectices_cor[:,1], vectices_cor[:,2], triangles = triangles, cmap=plt.cm.autumn)
ax_cor.view_init(elev = 10, azim = 25)


plt.show()
plt.savefig("ExplSolution.png")