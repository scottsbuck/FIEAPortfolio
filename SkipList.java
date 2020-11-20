// Scott Buck
// COP 3503 Spring 2020
// NID: sc091006

import java.util.*;

class Node<AnyType extends Comparable<AnyType>>
{
  // fields for the nodes.  Implemented the heights and levels of the nodes with
  // an ArrayList
  int height;
  AnyType data;
  ArrayList<Node<AnyType>> nextNodes;

  // initializes a node and adds null references to the ArrayList of nextNodes
  Node(int height)
  {
    this.height = height;
    nextNodes = new ArrayList<Node<AnyType>>(height);
    for (int i = 0; i <= height; i++)
    {
      nextNodes.add(null);
    }
  }
  // does the same as the above constructor just also initializes the data
  Node(AnyType data, int height)
  {
    this.data = data;
    this.height = height;
    nextNodes = new ArrayList<Node<AnyType>>(height);
    for (int i = 0; i <= height; i++)
    {
      nextNodes.add(null);
    }
  }

  public AnyType value()
  {
    return data;
  }

  // because nodes start their levels at 0 but a level 0 is a height of 1
  // added a +1 to the return value;
  public int height()
  {
    return height+1;
  }

  // retrives the node at the specific level of the ArrayList
  public Node<AnyType> next (int level)
  {
    if (level < 0 || level > height)
    {
      return null;
    }
    return nextNodes.get(level);
  }

  // suggested methods
  // sets the ArrayList level equal to given node
  public void setNext(int level, Node<AnyType> node)
  {
    nextNodes.set(level, node);
  }
  // must add a null reference when increasing the height of a node
  public void grow()
  {
    height+=1;
    nextNodes.add(null);
  }

  // performs a coin flip if it lands on 0 then the node grows
  public void maybeGrow()
  {
    Random rand = new Random();
    int flip  = rand.nextInt(2);
    if (flip == 0)
    {
      grow();
    }
  }

  // sets height and removes the node in the arraylist at that level until the
  // arraylist is trimmed down to correct height
  public void trim(int height)
  {
    for (int i = this.height; i > height; i--)
    {
      nextNodes.remove(i);
    }
    this.height = height;
  }

}

public class SkipList<AnyType extends Comparable<AnyType>>
{
  int nodes = 0;
  int height;
  Node<AnyType> head;

  // constructor for SkipList without given height starts at 1
  SkipList()
  {
    head = new Node<AnyType>(1);
    height = 1;
  }
  // constructor for SkipList with given height, implements node height at
  // height - 1 becuase SkipList starts at 1 while node levels start at 0
  SkipList(int height)
  {
    if (height < 1)
    {
      head =  new Node<AnyType>(1);
      this.height = 1;
    }
    else
    {
      head = new Node<AnyType>(height-1);
      this.height = height;
    }
  }

  public int size()
  {
    return nodes;
  }

  public int height()
  {
    return height;
  }

  public Node<AnyType> head()
  {
    return head;
  }

  // add the node, and check if the SkipList needs to grow so that the added node could
  // have a height of the new height
  public void insert(AnyType data)
  {
    nodes++;
    int maxHeight = getMaxHeight(nodes);
    if (maxHeight > this.height)
    {
      growSkipList();
    }
    // generates height and creates the nodes needed to sift through the SkipList
    int compareResult = 0;
    int nodeHeight = generateRandomHeight(maxHeight);
    Node<AnyType> newNode = new Node<AnyType>(data, nodeHeight);
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode;
    //height-1 becuase of node heights
    int currentHeight = height-1;
    // go until 0 because 0 is height 1
    // what this does is we have our temp node and a node that points to the next node at
    // the specified height.  if its null, we insert the node there if the height is right
    // if not then we compare values and work our way down the list until we find the right
    // spot to insert the node
    while(currentHeight >= 0)
    {
      nextNode = tempHead.next(currentHeight);
      if (nextNode == null)
      {
        if (currentHeight <= nodeHeight)
        {
          tempHead.setNext(currentHeight, newNode);
        }
        currentHeight--;
        continue;
      }
      compareResult = nextNode.data.compareTo(data);
      if (compareResult >= 0)
      {
        if (currentHeight <= nodeHeight)
        {
          newNode.setNext(currentHeight, tempHead.next(currentHeight));
          tempHead.setNext(currentHeight, newNode);
        }
        currentHeight--;
      }
      else if (compareResult < 0)
      {
        tempHead = tempHead.next(currentHeight);
      }
    }
  }
  // same exact method as above except node height is determined rather than random
  public void insert(AnyType data, int height)
  {
    nodes++;
    int maxHeight = getMaxHeight(nodes);
    if (maxHeight > this.height)
    {
      growSkipList();
    }
    int compareResult = 0;
    int nodeHeight = height-1;
    Node<AnyType> newNode = new Node<AnyType>(data, nodeHeight);
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode;
    int currentHeight = this.height-1;
    while(currentHeight >= 0)
    {
      nextNode = tempHead.next(currentHeight);
      if (nextNode == null)
      {
        if (currentHeight <= nodeHeight)
        {

          tempHead.setNext(currentHeight, newNode);

        }
        currentHeight--;
        continue;
      }
      compareResult = nextNode.data.compareTo(data);
      if (compareResult >= 0)
      {

        if (currentHeight <= nodeHeight)
        {

          newNode.setNext(currentHeight, tempHead.next(currentHeight));
          tempHead.setNext(currentHeight, newNode);
        }
        currentHeight--;
      }
      else if (compareResult < 0)
      {

        tempHead = tempHead.next(currentHeight);
      }
    }
  }

  // same initializations as in the insert method
  public void delete(AnyType data)
  {
    int compareResult = 0;
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode = tempHead;
    int currentHeight = height-1;
    int nodeHeight = 0;
    // check if the value you want to delete is even in the skiplist
    boolean set = contains(data);
    // have to go through the skiplist twice in the delete method. I had to know what the
    // height of the node to delete was in the case of a duplicate.  If the duplicate had a
    // taller node to the right then the skiplist got messy, so now we find the height of the
    // first node and can then properly delete the correct node
    if (set == true)
    {
      while (currentHeight >= 0)
      {
        if ((nextNode = tempHead.next(currentHeight)) != null)
        {
          compareResult = nextNode.data.compareTo(data);
        }
        if ((nextNode = tempHead.next(currentHeight)) == null)
        {
          currentHeight--;
        }
        else if (compareResult > 0)
        {
          currentHeight--;
        }
        else if (compareResult < 0)
        {
          tempHead = nextNode;
        }
        else if (compareResult == 0)
        {
          if (currentHeight == 0)
          {
            nodeHeight = nextNode.height;
          }
          currentHeight--;
        }
      }
      tempHead = head;
      currentHeight = height-1;
      while (currentHeight >= 0)
      {
        if ((nextNode = tempHead.next(currentHeight)) != null)
        {
          compareResult = nextNode.data.compareTo(data);
        }
        if ((nextNode = tempHead.next(currentHeight)) == null)
        {
          currentHeight--;
        }
        else if (compareResult > 0)
        {
          currentHeight--;
        }
        else if (compareResult < 0)
        {
          tempHead = nextNode;
        }
        else if (compareResult == 0)
        {
          if (currentHeight <= nodeHeight)
          {
            tempHead.setNext(currentHeight, nextNode.next(currentHeight));
          }
          currentHeight--;
        }
      }
      // check to see if the skiplist needs to be trimmed
      nodes--;
      int notMaxHeight = getNotMaxHeight(nodes);
      if (notMaxHeight < height)
      {
        height = notMaxHeight;
        head.height = notMaxHeight - 1;
        trimSkipList();
      }
    }
  }

  // simple contains method, works in the same way as the delete and insert methods
  // works its way through until it sees the correct data
  // compares data, if the data youre looking at is less than the node youre on
  // you just go to the next node without adjusting height
  // if its greater than adjust height and check again, repeat until node is found
  public boolean contains(AnyType data)
  {
    int compareResult = 0;
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode = tempHead;
    int currentHeight = height;
    while (currentHeight >= 0)
    {
      nextNode = tempHead.next(currentHeight);
      if (nextNode == null)
      {
        currentHeight--;
        continue;
      }
      compareResult = nextNode.data.compareTo(data);
      if (compareResult < 0)
      {
        tempHead = nextNode;
      }
      else if (compareResult > 0)
      {
        currentHeight--;
      }
      else if (compareResult == 0)
      {
        return true;
      }
    }
    return false;
  }
  // same thing as the contains method but returns the node instead of returning true
  public Node<AnyType> get(AnyType data)
  {
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode = tempHead;
    int currentHeight = height;
    int compareResult = 0;
    while (currentHeight > 0)
    {
      if ((nextNode = tempHead.next(currentHeight)) != null)
      {
        compareResult = nextNode.data.compareTo(data);
      }
      if ((nextNode = tempHead.next(currentHeight)) == null)
      {
        currentHeight--;
      }
      else if (compareResult > 0)
      {
        currentHeight--;
      }
      else if (compareResult < 0)
      {
        tempHead = nextNode;
      }
      else if (compareResult == 0)
      {
        return nextNode;
      }
    }
    return null;
  }


  // depending on how many times the nodes count can be divided by 2 is my way of
  // finding log base 2 of n, then compare that to height
  private int getMaxHeight(int n)
  {
    int count = 1;
    for (double i = n; i > 2; i/=2)
    {
      count++;
    }
    if (count > height)
    {
      return count;
    }
    return height;
  }

  // same thing as the above method but is used in the delete method for checking if the
  // list needs to be trimmed
  private int getNotMaxHeight(int n)
  {
    int count = 1;
    for (double i = n; i > 2; i/=2)
    {
      count++;
    }
    if (count < height)
    {
      return count;
    }
    return height;
  }

  // generates a random height by flipping a coin, if it lands on 0 break the loop
  // and return the amount of times flipped on a 1
  private int generateRandomHeight(int maxHeight)
  {
    Random rand = new Random();
    int possibleHeight;
    for(possibleHeight = 0; possibleHeight < maxHeight-1; possibleHeight++)
    {
      int flip  = rand.nextInt(2);
      if (flip == 0)
      {
        break;
      }
    }
    return possibleHeight;
  }

  // performs all the steps of growing a skiplist
  // increase head height and skiplist height
  // then go through the oldMax height of the skiplist and do a maybegrow on each node
  // of that height
  private void growSkipList()
  {
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode = tempHead;
    head.grow();
    height+=1;
    Random rand = new Random();
    int oldMax = height-1;
    while((nextNode = tempHead.next(oldMax)) != null)
    {
      nextNode.maybeGrow();
      tempHead = nextNode;
    }
  }

  // trims the skiplist down to the new height
  // then also trims the nodes that were all too tall
  private void trimSkipList()
  {
    Node<AnyType> tempHead = head;
    Node<AnyType> nextNode = tempHead;
    while((nextNode = tempHead.next(height-1)) != null)
    {
      nextNode.trim(height-1);
      tempHead = nextNode;
    }
  }

  public static double difficultyRating()
  {
    return 4.0;
  }

  public static double hoursSpent()
  {
    return 16.0;
  }
}
