# Unit 2: Output an Image
import os
import cv2
import matplotlib.pyplot as plt
from point_and_vec import color
import math
"""TODO: 
- set up venv
- set up git repo
- start unit 3
- figure out branching so that each unit is finished on its own branch (end of unit 3 calls for big refactor, do this NOW)
- do linting / style checking
"""

def progress_indicator(base_text: str,start_value: int,current_value: int) -> None:
    """overrides previous string with current value + spaces based on length of initial value"""
    # used to indicate progress of loop
    def get_digits(number):
        '''helper function that outperforms len(str(number))'''
        # https://stackoverflow.com/questions/2189800/how-to-find-length-of-digits-in-an-integer
        if number > 0:
            return int(math.log10(number))+1
        elif number == 0:
            return 1
        else:
            raise ValueError("progress indicator should not reach negative numbers")
    spaces_necessary = get_digits(start_value)-get_digits(current_value)
    print("\r%s%d" % (base_text,current_value)+" "*spaces_necessary,end="",flush=True)

def hello_world_ppm(output_ppm_dir: str = ".") -> str:
    """generates PPM file, sent to directory given in param, returns path"""
    image_width : int =  256
    image_height : int = 256
    output_ppm_path = os.path.abspath(output_ppm_dir)+'\\image.ppm'

    with open(output_ppm_path,"w+") as f:
        f.write("P3\n%d %d\n255\n" % (image_width,image_height))
        for j in range(image_height):
            progress_indicator("scanlines remaining: ",image_height,image_height-j)
            for i in range(image_width):
                rgb = [i/(image_width-1),j/(image_height-1),0]
                pixel_colour = color(rgb)
                f.write(pixel_colour.write_colour())
    print("\nDone.") #newline necessary because output stream after progress_indicator call is pointing to end of previous line, not newline
    return output_ppm_path

def view_ppm_img(img_path: str):
    img = cv2.imread(img_path)
    img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
    plt.imshow(img)
    plt.show()

if __name__ == "__main__":
    ppm_path = hello_world_ppm("images")
    view_ppm_img(ppm_path)