# pleb
A random social network generator, with randomly generated nodes and edges attributes.

A quick description of the algortihm
=

1. Generates a fixed number of independant nodes, with random attributes according to different
statistical laws (e.g age distribution), names and surname picked from external files.

2. Generates family links between the nodes, according to strict rules, such as no interbreeding,
parents at least 18yr older that their children, etc. Propagates family names accordingly (french rule:
wife takes the name of her husband, children take the name of their father). Some single-parent families
may occur but much less often than two-parents families. No child is left orphan.

3. Generates a skeleton of abstract communities from a template xml file

4. Populates those communities with the nodes, according to strict rules (some positions in certain communities
may require a minimum or maximum age, etc.)

5. WIP: generates a map to localize communities and nodes

6. TODO: generates a schedule to simulate the displacement of the nodes in the map, going to their work, coming
back home, etc.

7. TODO: information transit between nodes

Long term goal: to simulate a simple "society", for use in simulations (epidemiology, information streams) or in 
games. The original idea was a spy game with a deep and rich gameplay.
