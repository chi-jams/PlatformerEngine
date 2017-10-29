levelFile = open( 'level7.txt', 'w' )

width = 30
height = 40

# Create an array of the map to generate hit boxes.
#level = [ [ 0 for x in range( width ) ] for y in range( height ) ]

# Put the height and width into the document first for the level.
levelFile.write( str( width ) + ' ' + str( height ) + '\n' )

# Write the layout of the level into the text document,
# and also write the layout of the level into the array so that we
# can make all the hit boxes.
for y in range( 0, height ):

	for x in range( 0, width ):

		if y == height - 3 and x == 2:
			levelFile.write( '9 ' )
		elif y == 0 or x == 0 or y == height - 1 or x == width - 1:
			levelFile.write( '1 ' )	
			#level[x][y] = 1
		else:
			levelFile.write( '0 ' )

	levelFile.write( '\n' )


levelFile.close()

'''
# After we have a picture of the level in side the array level,
# start forming hit boxes for all the platforms in the level.
for y in range( 0, height ):

	for x in range( 0, width ):

		if level[x][y] == 1:

            # The number of tiles that are horizontally adjacent.
            numBoxes = 1

            # Get all the tiles  that are adjacent to each other in the same row.
            while level[ x + numBoxes ][y] == 1:
                numBoxes++
            
            # Once we find the last horizontally adjacent solid tile, create a hit box that
            # covers all of those horizontally adjacent tiles.
            hitBoxes.push_back( Rect( x * tileSize, y * tileSize, numBoxes * tileSize, tileSize ) );

            for( int i = 0; i < numBoxes; i++ )
            {
                // Get the last hit box on the hit box vector, because it's the one that we just
                // just created above this, and get all of the tiles contained in the hit box to point
                // to this hit box. 
                level[ x + i ][y].setHitBox( hitBoxes.size() - 1 );
            }

            // When everything's done, we don't want to go over those tiles to be on another hit box,
            // so we skip over them.
            x += numBoxes;
'''