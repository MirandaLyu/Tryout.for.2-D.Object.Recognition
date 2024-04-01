# tryout-for-2D-object-recognition

This was a CV class project as well. But it took me a lot of time to build.

Good amounts of time were spent on reading textbooks (trying to understand the mechanisms of thresholding, morphological operations, the least central moment, etc.) and reading OpenCV documentations (understanding how to correctly use some functions).

## general design

My object recognition system was designed as a command line program.

It takes in a folder of images and either saves the objects' features to databse OR gives unknown objects matching labels.

*All images are white background and contain only one object

## first, thresholding the image

There are actually a lot of ways to find a relatively optimal threshold for an image. I have tried many and I found Otsu’s Method work best with my training images.

So I preprocessed every image a little first (blurring a little and making strongly colored pixels be darker) and then applied Otsu’s Method to binary images from scratch (not using the built-in function).

Here are examples of the original vs thresholded images:

<img width="200" alt="Screenshot 2024-03-31 at 9 12 20 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/93cccc8a-70fe-4743-8a32-516fb2559402">
<img width="100" alt="Screenshot 2024-03-31 at 9 12 32 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/f80183e6-1120-4141-8fb4-ccab12be6aa6">

<img width="200" alt="Screenshot 2024-03-31 at 9 12 41 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/d3ef21c1-2e95-452f-b736-e8dbd96033ee">
<img width="100" alt="Screenshot 2024-03-31 at 9 12 50 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/67edfd39-db11-4009-b40b-280e86d5ac42">

## second, clean up

Cleaning up is a very skilled technique. It can use layers of different morphological operations to deal with complicated shapes.

But since here my thresholded images are already doing a good job, I just used the Closing operation to fill the small holes in images.

Results:

<img width="200" alt="Screenshot 2024-03-31 at 9 25 25 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/8e492630-2d85-4eb9-9926-353c89a4bb19">
<img width="102" alt="Screenshot 2024-03-31 at 9 25 33 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/b4557546-9120-43af-b3aa-041a98c4cb18">

## third, segmentation

I ran the connectedComponentsWithStats() function on the cleaned up images, which gave me detailed information of each connected region in an image (such as area, centroids).

Since I just want the central object to be colored and show, I let the system find 3 largest region areas first, and then choose the one which is closest to the center of the image.

And these are the colored results:

<img width="200" alt="Screenshot 2024-03-31 at 11 56 54 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/d0d7ae05-5fc0-4e9e-827a-c322fded682e">
<img width="102" alt="Screenshot 2024-03-31 at 11 57 02 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/faefa32d-bff6-4cd1-a10e-c5f8fb57afaf">

## fourth, the axis of least central moment and the oriented bounding box

Here is to better locate objects. The axis of least central moment was drawn by calculating the orientation angle first. The oriented bounding box was drawn using minAreaRect() function, showing the smallest rectangle that encompasses the object.

*I actually need more time to understand what moments really are

Results:

<img width="200" alt="Screenshot 2024-04-01 at 12 12 14 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/89afcf8e-e8e9-452e-b21b-b9dff790694e">
<img width="102" alt="Screenshot 2024-04-01 at 12 12 44 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/b0b614f1-304b-4ccf-b351-1dc5f3ec9b87">

## fifth, get mathematical features

In computer's intelligence, every object is represented by a set of features. So I computed "percentFilled", "boundingBoxRatio", "perimeter", and "circularity" for each object and if you want to save these features to database, the system will ask you for a label/name.

This is an example database file I got:

<img width="220" alt="Screenshot 2024-04-01 at 12 23 06 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/ebd466e1-191c-4af4-8cbd-125e23cfcf5b">

## sixth, identify

So after calculating all the features as in the last step, if you want to identify an object, the system will compare its features with each object in the database (using the scaled Euclidean distance) and show the smallest-distance matching label on the image.

Examples:

<img width="120" alt="Screenshot 2024-04-01 at 12 28 57 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/39ac69ec-9f0c-46ad-9a01-775286916acb">
<img width="120" alt="Screenshot 2024-04-01 at 12 29 07 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/467f388b-7644-4ce7-a226-f777536b9383">
<img width="120" alt="Screenshot 2024-04-01 at 12 29 14 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/7345b400-542b-4f81-a273-e061f19e3933">

## FINAL THOUGHTS

Because of the time constriant, my tryout stopped up to this point. There are lots of things that can continue trying in this area, like implementing a new classifier, letting an image has more than one object, turning the whole system into a real-time video one, etc.

When I was doing the training, some objects were actually more complicated than the ones shown above: 

<img width="63" alt="Screenshot 2024-04-01 at 12 39 33 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/929295d9-237a-452b-b45a-d81d5bae8955">
<img width="62" alt="Screenshot 2024-04-01 at 12 39 42 AM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/55d3fd49-32be-4ef8-b20b-1802d95010ea">

Real life situations must have more variations.

But it's fun to experience the whole process and learn more about what is object detection.

