<table>
    <tr>
        <td><label for="tag">Tag:</label></td>
        <td><input type="text" name="tag" id="tag" size="30" value="<?php echo $_POST['tag']?>" /></td>
        <?php if( isset($_POST['insert']) && ( empty($_POST['tag']) ) ) : ?>
            <td style="color:red">The Tag must not be empty.</td>
            <?php endif; ?>
        <?php if( isset($_POST['insert']) && ( substr_count($_POST['tag'], ' ') ) ) : ?>
            <td style="color:red">The Tag must not contain whitespaces.</td>
            <?php endif; ?>
    </tr>
    <tr>
        <td><label for="address">Address:</label></td>
        <td><input type="text" name="address" id="address" size="30" value="<?php echo $_POST['address']?>" /></td>
        <?php if( isset($_POST['insert']) && ( empty($_POST['address']) ) ) : ?>
            <td style="color:red">The Address must not be empty.</td>
            <?php endif; ?>
        <?php if( isset($_POST['insert']) && ( substr_count($_POST['address'], ' ') ) ) : ?>
            <td style="color:red">The Address must not contain whitespaces.</td>
            <?php endif; ?>        
    </tr>
    <tr>
        <td><label for="tooltip">Tooltip:</label></td>
        <td><input type="text" name="tooltip" id="tooltip" size="30" value="<?php echo $_POST['tooltip']?>" /></td>                    
    </tr>
    <tr>
        <td colspan="3"><input style="width:258px;" type="submit" name="insert" value="Insert" /></td>
    </tr>
    </table>
